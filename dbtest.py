#!/bin/env python3

# Basic idea is to understand the COMP database structure and be able to read
# it into a proper SQL database, maybe SQLite or PostgreSQL
import pyodbc
import pdb
import glob

def main():
    open_database() 
    return

def open_database():
    
    # Directory to where ever COMP tables (ADT, ADM, ADI) are stored
    # Had a weird issue earlier where some tables had lowercaser extensions...
    comp_directory = '/home/john/Comp4win'

    # Advantage ODBC driver fails is autocommit isn't set
    # DSN is set in /etc/odbc.ini
    conn = pyodbc.connect('DSN=Comp', autocommit=True)
    cur = conn.cursor()

    tables = glob.glob(comp_directory + "/*.ADT")

    for table in tables:

        # This sucks but I can't seem to pass a param to execute?
        cur.execute("SELECT TOP 1 * FROM [" + table + "]")

        # Tuple containing:
            # column name (or alias, if specified in the SQL)
            # type code
            # display size (pyodbc does not set this value)
            # internal size (in bytes)
            # precision
            # scale
            # nullable (True/False)

        table_desc = cur.description

        pdb.set_trace()

    return

if __name__ == '__main__':
    main()
