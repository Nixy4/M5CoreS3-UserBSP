set(src_dirs        
  "./"
  "./lv_imgs/"
  "./apps/launcher/"
  # "./apps/faceRecognition/"
  )

set(include_dirs    
  "./"
  "./lv_imgs/"
  "./apps/"
  "./apps/launcher/"
  # "./apps/faceRecognition/"
  )

set(requires        
  mooncake
  M5CoreS3-UserBSP
  m5stack_core_s3
  )

idf_component_register(
  SRC_DIRS ${src_dirs} 
  INCLUDE_DIRS ${include_dirs}
  REQUIRES ${requirs}
  )

component_compile_options(-ffast-math -O3)

