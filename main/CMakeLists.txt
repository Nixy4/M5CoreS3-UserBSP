set(src_dirs        
  "./"
  "./apps/launcher/"
  "./apps/faceRecognize/"
  )

set(include_dirs    
  "./"
  "./apps/launcher/"
  "./apps/faceRecognize/"
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

