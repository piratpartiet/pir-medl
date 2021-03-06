PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE synced (
  file TEXT
    CONSTRAINT synced_file_length
      CHECK (length(file) > 0)
    UNIQUE
    NOT NULL
  ,
  orig TEXT
  ,
  rev TEXT
    CONSTRAINT synced_rev_length
      CHECK (length(rev) = 40 OR rev = '')
  ,
  date TEXT
    CONSTRAINT synced_date_length
      CHECK (date IS NULL OR length(date) = 19)
    CONSTRAINT synced_date_valid
      CHECK (date IS NULL OR datetime(date) IS NOT NULL)
);
INSERT INTO "synced" VALUES('src/Makefile','Lib/std/c/Makefile','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 01:07:30');
INSERT INTO "synced" VALUES('src/medl.c','Lib/std/c/std.c','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 01:07:30');
INSERT INTO "synced" VALUES('src/medl.h','Lib/std/c/std.h','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 01:07:30');
INSERT INTO "synced" VALUES('src/t/medl.t','Lib/std/perl-tests-tab','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 02:10:49');
CREATE TABLE todo (
  file TEXT
    CONSTRAINT todo_file_length
      CHECK(length(file) > 0)
    UNIQUE
    NOT NULL
  ,
  pri INTEGER
    CONSTRAINT todo_pri_range
      CHECK(pri BETWEEN 1 AND 5)
  ,
  comment TEXT
);
COMMIT;
