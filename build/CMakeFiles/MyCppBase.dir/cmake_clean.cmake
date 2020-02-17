file(REMOVE_RECURSE
  "MyCppBase.pdb"
  "MyCppBase"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/MyCppBase.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
