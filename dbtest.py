#!/bin/env python3

# Basic idea is to understand the COMP database structure and be able to read
# it into a proper SQL database, maybe SQLite or PostgreSQL
import pyodbc
import pdb
import glob
import sqlite3
import datetime

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
        cur.execute("SELECT COUNT(*) FROM [" + table + "]")
        rowcount = cur.fetchone()[0]
        cur.execute("SELECT TOP 1 *  FROM [" + table + "]")

        # Tuple containing:
            # column name (or alias, if specified in the SQL)
            # type code
            # display size (pyodbc does not set this value)
            # internal size (in bytes)
            # precision
            # scale
            # nullable (True/False)

        table_desc = cur.description
        print(table.replace(comp_directory + "/", "").replace(".ADT", ""), end=" ")
        print(rowcount)

        for column in table_desc:
            print("\t" + column[0], end=" ")
            if column[1] == type(""):
                print("string")
            elif column[1] == type(False):
                print("bool")
            elif column[1] == type(0.0):
                print("float")
            elif column[1] == datetime.date:
                print("datetime.date")
            elif column[1] == type(0):
                print("int")
            elif column[1] == type(bytearray()):
                print("bytearray")

            else:
                pdb.set_trace()

    return

if __name__ == '__main__':
    main()
