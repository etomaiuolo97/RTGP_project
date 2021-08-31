GLuint textureCube;

glm::vec3 lightPos[] = {glm::vec3(0.0f, 0.0f, 10.0f)};

GLfloat Eta = 1.0f/1.52f;
GLfloat mFresnelPower = 5.0f;

GLfloat diffuseColor [] = {1.0f, 1.0f, 1.0f};
GLfloat specularColor [] = {1.0f, 1.0f, 1.0f};
GLfloat ambientColor [] = {0.1f, 0.1f, 0.1f};

GLfloat Kd = 0.5f;
GLfloat Ks = 0.4f;
GLfloat Ka = 0.1f;

GLfloat shininess = 25.0f;
GLfloat alpha = 0.2f;
GLfloat F0 = 0.9f;

glm::mat3 createNormalMatrix (glm::mat4 view, glm::mat4 modelMatrix) {
    glm::mat3 matrix = glm::mat3(1.0f);
    glm::inverseTranspose(glm::mat3(view * modelMatrix));
    return matrix;
}

glm::mat4 createTransformationMatrix(glm::vec3 translation, GLfloat rx, GLfloat ry, GLfloat rz, GLfloat scale) {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix, (GLfloat)glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, (GLfloat)glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, (GLfloat)glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

    return matrix;
}

glm::mat4 createViewMatrix(Camera camera) {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, (GLfloat)glm::radians(camera.roll), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 cameraPos = camera.position;
    glm::vec3 negCameraPos = glm::vec3(-cameraPos.x, -cameraPos.y, -cameraPos.z);
    matrix = glm::translate(matrix, negCameraPos);

    return matrix;
}

void LoadTextureCubeSide (string path, string side_img, GLuint side_name) {
    int w, h;
    unsigned char * image;
    string fullname = path + side_img;

    image = stbi_load(fullname.c_str(), &w, &h, 0, STBI_rgb);
    if (image == nullptr)
        std::cout << "Failed to load texture!" << std::endl;
    
    glTexImage2D(side_name, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
}

GLint LoadTextureCube (string path) {
    GLuint textureImage;

    glGenTextures(1, &textureImage);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureImage);

    LoadTextureCubeSide(path, std::string("posx.jpg"), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    LoadTextureCubeSide(path, std::string("negx.jpg"), GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    LoadTextureCubeSide(path, std::string("posy.jpg"), GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    LoadTextureCubeSide(path, std::string("negy.jpg"), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    LoadTextureCubeSide(path, std::string("posz.jpg"), GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    LoadTextureCubeSide(path, std::string("negz.jpg"), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureImage;
}