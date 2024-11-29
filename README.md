# Database-Management-System

I'm here to build a working single-user DBMS that can execute certain simple SQL queries. 
The objective is to learn how a DBMS is organized and what goes on inside it when queries are executed. 

- The topmost layer is a parser that parses SQL queries and calls appropriate functions in the lower layers to perform relational operations.
- The lowermost layer is the disk I/O layer which reads and writes pages from and to the disk. In your case, the disk will just be the UNIX file system.
- I implement a buffer manager, heap files - a collection of pages that are used to hold a relation, the database catalogs and various utilities and several relational operators.

This miniature DBMS can handle a fairly useful set of queries
