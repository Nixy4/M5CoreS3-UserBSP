set(src_dirs        
  "./"
  "./apps/launcher/"
  "./apps/faceRecognition/"
  )

set(include_dirs    
  "./"
  "./apps/"
  "./apps/launcher/"
  "./apps/faceRecognition/"
  )

set(requires        
  mooncake
  M5CoreS3-UserBSP
  )

idf_component_register(
  SRC_DIRS ${src_dirs} 
  INCLUDE_DIRS ${include_dirs}
  REQUIRES ${requirs}
  )

component_compile_options(-ffast-math -O3)

