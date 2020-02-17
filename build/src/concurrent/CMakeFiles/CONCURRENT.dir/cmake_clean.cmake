file(REMOVE_RECURSE
  "libCONCURRENT.pdb"
  "libCONCURRENT.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/CONCURRENT.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
