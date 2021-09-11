#pragma once

#include <glm/glm.hpp>

class Transform {
public:

    glm::vec3 position, rotation, scale;
	glm::mat4 translationMatrix, rotationMatrix, scalingMatrix;
	glm::mat4 matrix;

	Transform(){
        this->position = this->rotation = glm::vec3();
        this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
        this->matrix = this->translationMatrix = this->rotationMatrix = this->scalingMatrix = glm::mat4(1.0f);
    }
	~Transform(){}

	void TranslateTo(const glm::vec3& position){
        TranslateBy(position - this->position);
    }

	void ScaleTo(const glm::vec3& scale){
        ScaleBy(scale / this->scale);
    }

	void RotateTo(const glm::vec3& rotation){
        RotateBy(rotation.x - this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        RotateBy(rotation.y - this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        RotateBy(rotation.z - this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    }

	void TranslateBy(const glm::vec3& distance){
        translationMatrix = glm::translate(translationMatrix, distance);
        matrix = translationMatrix * rotationMatrix * scalingMatrix;
        this->position += distance;
    }

	void ScaleBy(const glm::vec3& scale){
        scalingMatrix = glm::scale(scalingMatrix, scale);
        matrix = translationMatrix * rotationMatrix * scalingMatrix;
        this->scale *= scale;
    }

	void RotateBy(float angle, const glm::vec3& axis){
        rotationMatrix = glm::rotate(rotationMatrix, angle, axis);
        matrix = translationMatrix * rotationMatrix * scalingMatrix;
        this->rotation += axis * angle;

        this->rotation.x = fmod(this->rotation.x + 360.0f, 360.0f);
        this->rotation.y = fmod(this->rotation.y + 360.0f, 360.0f);
        this->rotation.z = fmod(this->rotation.z + 360.0f, 360.0f);
    }
};