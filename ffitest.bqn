sqlite ← •Import "bqnsqlite.bqn"
! sqlite.Open "test.db" # should return true
•Show x ← sqlite.Select "select x from test" # test selection works
•Show y ← sqlite.Select "select y from test LIMIT 10" # test sql query is being read
! ¬ sqlite.Open "fake.db"