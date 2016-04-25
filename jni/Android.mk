LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= GraphicsEngine

LOCAL_SRC_FILES := opengl_jni_Natives.cpp open.cpp Renderer.cpp Camera.cpp Cube.cpp DocumentSearchHelpers.cpp Helpers.cpp MeshData.cpp MeshMultipleTextures.cpp ModelPositioning.cpp objMtlLoader.cpp ShaderDetails.cpp ShaderFactory.cpp ShaderHandler.cpp ThreeDObjectBase.cpp Vertex.cpp WaterObject.cpp Ship.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_LDLIBS    := -lGLESv2 -llog -landroid -ldl 
#-lEGL  -lGLESv1_CM  

LOCAL_CPPFLAGS := -std=gnu++0x

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

include $(BUILD_SHARED_LIBRARY)
