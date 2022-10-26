import pymysql_config
import threading
import pymysql

DATETIME_FORMAT = "%Y-%d-%m %H:%M:%S"

class PymysqlUtil:
    @staticmethod
    def get_connect():
        thread_local = threading.local()
        if not hasattr(thread_local, 'db'):
            connect = pymysql.connect(host=pymysql_config.host,
                                      port=pymysql_config.port,
                                      user=pymysql_config.user,
                                      passwd=pymysql_config.passwd,
                                      db=pymysql_config.db)
            cursor = connect.cursor()
            db = {
                'connect': connect,
                'cursor': cursor,
            }
            thread_local.db = db

        return thread_local.db

    @staticmethod
    def close_connect():
        thread_local = threading.local()
        if hasattr(thread_local, 'db'):
            db = thread_local.db
            db.cursor.close()
            db.connect.close()
            threading.local().__dict__.clear()

    @staticmethod
    def columns_name_of_index_map(cursor):
        results = {}
        column = 0
        for d in cursor.description:
            results[d[0]] = column
            column = column + 1

        return results
