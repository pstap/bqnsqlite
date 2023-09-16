# open_ffi :: str -> bool
open_ffi←"libbqnsqlite.so" •FFI "i64"‿"bqnsqlite_open"‿"a"

# select_ffi :: str -> [int]
select_ffi←"libbqnsqlite.so" •FFI "a"‿"bqnsqlite_select"‿"i64"‿"a"

Open⇐{Open_ffi ⟨𝕩⟩}
Select⇐{Select_ffi ⟨𝕨, 𝕩⟩}

