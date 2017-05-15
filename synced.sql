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
INSERT INTO "synced" VALUES('src/medl-add','Lib/std/perl-tab','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 00:04:17');
INSERT INTO "synced" VALUES('src/medl-add.c','Lib/std/c/std.c','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 01:07:30');
INSERT INTO "synced" VALUES('src/medl-add.h','Lib/std/c/std.h','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 01:07:30');
INSERT INTO "synced" VALUES('src/medl-init','Lib/std/sh','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-14 21:11:29');
INSERT INTO "synced" VALUES('src/t/medl-add.t','Lib/std/perl-tests-tab','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-15 00:04:31');
INSERT INTO "synced" VALUES('src/t/medl-init.t','Lib/std/perl-tests-tab','38a62d4427a4c364b22a6007a862ba2e73eaf9ad','2017-05-14 21:10:58');
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
