handle_type ← "i64"

# open_ffi :: str -> bool
open_ffi←"libbqnsqlite.so" •FFI handle_type‿"bqnsqlite_open"‿"a"

# select_ffi :: str -> [int]
select_ffi←"libbqnsqlite.so" •FFI "a"‿"bqnsqlite_select"‿handle_type‿"a"

# close_ffi :: handle -> ()
close_ffi←"libbqnsqlite.so" •FFI ""‿"bqnsqlite_close"‿handle_type

Open⇐{Open_ffi ⟨𝕩⟩}
Select⇐{Select_ffi ⟨𝕨, 𝕩⟩}
Close⇐{Close_ffi ⟨𝕩⟩}

