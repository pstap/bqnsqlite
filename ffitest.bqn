sqlite ← •Import "bqnsqlite.bqn"
test_db ← sqlite.Open "test.db" # should return true
•Show test_db
•Show x ← test_db sqlite.Select "select x from test" # test selection works
•Show y ← test_db sqlite.Select "select y from test LIMIT 10" # test sql query is being read
! ¬ sqlite.Open "fake.db"
