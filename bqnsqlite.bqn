# open_ffi :: str -> bool
open_ffi←"libbqnsqlite.so" •FFI "u8"‿"bqnsqlite_open"‿"a"

# select_ffi :: str -> [int]
select_ffi←"libbqnsqlite.so" •FFI "a"‿"bqnsqlite_select"‿"a"

Open⇐{Open_ffi ⟨𝕩⟩}
Select⇐{Select_ffi ⟨𝕩⟩}
