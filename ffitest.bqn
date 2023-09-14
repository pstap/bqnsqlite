open←"libbqnsqlite.so" •FFI ""‿"bqnsqlite_open"
sel←"libbqnsqlite.so" •FFI "a"‿"bqnsqlite_select"‿"a"
printcol←"libbqnsqlite.so" •FFI "a"‿"read_x"
Open⟨⟩
# •Show 1+PrintCol⟨⟩
•Show Sel ⟨"select x+100 from test"⟩
