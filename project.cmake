add_library(${ProjectName} OBJECT)
target_import_src(${ProjectName})

target_import_freertos(${ProjectName} PUBLIC)
target_import_base(${ProjectName} PUBLIC)
