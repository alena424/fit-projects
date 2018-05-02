#include<student/program.h>

#include<stdlib.h>
#include<stdio.h>

GLuint compileShader(GLenum const type,char const*source){
  //reserve shader id
  GLuint id = glCreateShader(type);

  //set shader sources
  glShaderSource(
      id     ,
      1      ,
      &source,
      NULL   );

  //compile shader
  glCompileShader(id);

  //get compilation status
  GLint status;
  glGetShaderiv(id,GL_COMPILE_STATUS,&status);
  if(status == GL_TRUE)return id;

  //get compilation log
  GLint bufferLen;
  glGetShaderiv(id,GL_INFO_LOG_LENGTH,&bufferLen);
  if(bufferLen>0){
    char*buffer = (char*)malloc((size_t)bufferLen*sizeof(char));
    glGetShaderInfoLog(id,bufferLen,NULL,buffer);
    fprintf(stderr,"%s",buffer);
    free(buffer);
    return 0;
  }
  return id;
}

GLuint linkProgram(GLuint const shader0,GLuint const shader1){
  //reserver program id
  GLuint id = glCreateProgram();

  //attach all shaders
  glAttachShader(id,shader0);
  glAttachShader(id,shader1);

  //link program
  glLinkProgram(id);

  //mark shaders for deletion
  glDeleteShader(shader0);
  glDeleteShader(shader1);

  //get linking status
  GLint status;
  glGetProgramiv(id,GL_LINK_STATUS,&status);
  if(status == GL_TRUE){
    return id;
  }

  //get linking log
  GLint bufferLen;
  glGetProgramiv(id,GL_INFO_LOG_LENGTH,&bufferLen);
  if(bufferLen>0){
    char*buffer = (char*)malloc((size_t)bufferLen*sizeof(char));
    glGetProgramInfoLog(id,bufferLen,NULL,buffer);
    fprintf(stderr,"%s",buffer);
    free(buffer);
    glDeleteProgram(id);
  }
  return 0;
}




