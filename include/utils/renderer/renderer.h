#pragma once

#include <glad/glad.h>

#include <set>

#include <utils/renderer/renderable.h>
#include <utils/camera.h>

class Renderer {
private:
    Camera* camera;

    std::set<Renderable*> renderables;

    bool clipPlaneEnable;
    glm::vec4 clipPlane;

public:

    Renderer(Camera* camera){
        this->camera = camera;
        this->clipPlaneEnabled = false;
        MasterRenderer::Register(this);
    }

    ~Renderer(){
        renderables.clear();
	    MasterRenderer::Unregister(this);
    }

	void EnableClipPlane(glm::vec4 clipPlane){
        this->clipPlaneEnabled = true;
	    this->clipPlane = clipPlane;
    }

	void DisableClipPlane(){
        this->clipPlaneEnabled = false;
	    this->clipPlane = glm::vec4(0, -1, 0, 10000);
    }

	void Register(Renderable* renderable){
        renderables.insert(renderable);
    }

	void Unregister(Renderable* renderable){
        renderables.erase(renderable);
    }

	void Render(){
        if (clipPlaneEnabled)
		    glEnable(GL_CLIP_DISTANCE0);
	    else
		    glDisable(GL_CLIP_DISTANCE0);

        for (Renderable* renderable : renderables) {
            Shader* shader = renderable->shader;
            Texture* texture = renderable->texture;
            VertexArray* vArray = renderable->mesh->VAO;
            IndexBuffer* iBuffer = renderable->mesh->indices;

            shader->Bind();
            texture->Bind();
            vArray->Bind();
            iBuffer->Bind();

            shader->SetUniform4f("clipPlane", clipPlane);

            shader->SetUniform1f("textureTiling", renderable->GetTextureTiling());
            shader->SetUniform3f("directionalLight.color", Light::color);
            shader->SetUniform1f("directionalLight.intensity", Light::intensity);
            shader->SetUniform3f("directionalLight.direction", Light::direction);
            shader->SetUniformMatrix4fv("gProj", camera->GetProjectionMatrix());
            shader->SetUniformMatrix4fv("gCamera", camera->GetViewMatrix());
            shader->SetUniformMatrix4fv("gWorld", renderable->GetTransform()->GetMatrix());

            glDrawElements(GL_TRIANGLES, iBuffer->GetCount(), GL_UNSIGNED_INT, 0);

            iBuffer->Unbind();
            vArray->Unbind();
            texture->Unbind();
            shader->Unbind();
        }
	
    }
};