set(src_dirs        
  . 
  )

set(include_dirs    
  . 
  )

set(requirs        
  m5stack_core_s3
  )

idf_component_register(
  SRC_DIRS ${src_dirs} 
  INCLUDE_DIRS ${include_dirs}
  REQUIRES ${requirs}
  )