/****************************************************************
*                                                               * 
*   Copyright (c) 2011 by Founder Corporation.                  * 
*                                                               * 
*   All Rights Reserved.                                        * 
*                                                               * 
****************************************************************/
/// @file XdbExceptions.h
/// @brief Defines XdbBaseException and its sub-classes.
///
/*
History:
  <------------------  -------  --------------------------->
    Document-usr-nn    yyMmmdd  Description of Change
  <------------------  -------  --------------------------->
          zhaow        11/06/07  Created.
          
          Mingchen WEI 11/09/05  Revised.   
            &
          Hao LI        
          
          ..           11/09/07  Revised. the structure of exception.  
          Minchen WEI
            &
          Hao LI       11/09/15 Revised. fixed up the warning:"Warning: Defined but not used" on g++/gcc compiler and initialization order warning..
          
          Hao LI       11/10/14 Revised. Changes the definition formation of error message arraies from const char name[x][x] to const char* name[x].
          
          Lijuan Chen       11/10/14 Revised.
*/

/*  
*  IMPORTANT INFORMATION:
*     the first element (index 0) of all the error information array will not be used in system.
*     it's reserved by system. the first available item index will start from 1. 
*  Question: How to use the exceptions in our system?
*  Answer:   As we can see, the previous exception structure has been changed to this one. Each error code correspond to an exception.
*            There are some tricks in this part. Firstly, the error messages have been divided into 6 arraies---Error_Message_Net,Error_Message_DB
*            Error_Message_StorageEngine, Error_Message_QueryEngine, Error_Message_Other, Error_Message_Array_Execution, so does the errocodes.
*            If the system needs a new exception, first of all, a new error code MUST be added into. Therefore, we can throw an exception by its errorcode in future usage.
*            Secondly, after the adding the new error code, the error message associated with errorcode also MUST be added into its error message array.
*            Thirdly,  you should implement a new class (named as xxxException). The new added class is used to describe what exception happens in our code.
*            Finally,  in function, ThrowExceptionsByCode, a new line will be added to make sure that exception could be thrown by using its errorcode.
*  Question: How many exceptions do we have in a section.
*  Answer:   About 1000 exceptions, pleas make sure that you have less than 1000 exceptions in your system, :-).
*   
*/

#ifndef _XDB_EXCEPTIONS_H_
#define _XDB_EXCEPTIONS_H_

#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "XdbCommon.h"


namespace FounderXDB {
namespace EXCEPTION {

typedef int32 ErrorCode_t;

#define THROW_E(EXCEPT_DESC) throw #EXCEPT_DESC
#define LEN_DIMENSION_0 256 

class InvalidUserException;

//the exception messages for the  network. the change reason refers to 'History'. 
//static const char Error_Message_Net [][LEN_DIMENSION_0] = {  //without warning on linux with g++ compliler.
static const char* Error_Message_Net [] = {    //with warning on linux with g++ compliler.
     ("It is reserved by system, and do not use it directly...\n"),
     ("The IP adddress of the server is invalid for connection. Please check it out again...\n") , /*ERR_NET_INVALIDE_IP*/
     ("The server is invalid for connection, please check it out...\n"),                    /*ERR_NET_SERVER_NOT_OPEN*/
     ("The connection timed out...\n"),                                                    /*ERR_NET_TIMEOUT*/
     ("The certificates are invalid.Please check them out again...\n"),                    /*ERR_NET_INVALID_CERTIFICATES*/
     ("Operation server framework exception: unrecognized command...\n"),                     /*ERR_UNKNOWN_COMMAND */
     ("The connection is closed...\n"),                                                     /*ERR_NET_CONNECTION_CLOSED*/
     ("The transaction does not exist...\n"),                                              /*ERR_NET_NO_TRANSACTION*/
     ("The executor's request for creating failed...\n"),                                           /*ERR_NET_EXECUTOR_REQUEST_CREATE_FAILED*/
     ("The user does not login...\n"),                                                      /*ERR_NET_NOT_LOGIN*/                              
     ("A socket exception occurs...\n"),                                                  /*ERR_SOCKET_EXCEPTION*/
     ("The Server Address:%s,%d is in use...\n"),                                           
     ("The transaction also exist...\n"),
     ("Some of the data node machines are down...\n"),
     ("SSL error...\n") ,                                                                 //ERR_NET_SSL
     ("Can not do recursive login...\n"),                                                    //ERR_NET_RECURSIVE_LOGIN
	 ("using server config file not correctly...\n"),
	 ("Cluster environment expection...\n")
} ;

//the exception messages for the db.
//static const char Error_Message_DB[][LEN_DIMENSION_0]= {
static const char* Error_Message_DB[]= {
    ("It is reserved by system, and do not use it directly...\n"),                                    
    ("You have input an invalid user name...\n"),                                           /*ERR_DB_INVALIDE_USER*/
    ("You have input an invalid password for login user...\n"),                             /*ERR_DB_INVALIDE_PASSWORD*/ 
    ("The inquiry statement is invalid. Please check it again...\n"),                       /*ERR_DB_INVALIDE_STATEMENT*/
    ("The user already exists...\n"),                                                       /*ERR_DB_USER_EXISTS*/  
    ("The user does not exist...\n"),                                                       /*ERR_DB_USER_NOT_EXISTS*/   
    ("The role already exists...\n"),                                                       /*ERR_DB_ROLE_EXISTS*/
    ("The role does not exist...\n"),                                                       /*ERR_DB_ROLE_NOT_EXISTS*/
    ("You are not authorized to perform this operation...\n"),                               /*ERR_DB_NOT_AUTHORIZED*/
    ("Database Not Used.\n")                                                                 //ERR_DATABASE_NOT_USED

};                              

//the exception messages for the storage engine.
//static const char Error_Message_StorageEngine[][LEN_DIMENSION_0]={
static const char* Error_Message_StorageEngine[]={
    ("It is reserved by system, and do not use it directly...\n"),
    ("Can not access the query engine...\n") ,
	("Dead lock dected...\n"),
	("Serializable check failed, this transaction has been aborted, please try again...\n"),
	("Can not drop the currently using object...\n"),
    ("Heap tuple updated concurrently...\n"),
	("Talespace already exists...\n"),
	("Can not execute update in standby server...\n"),
    ("Two masters found...\n"),
    ("Unqiue violation...\n"),
	("Object not exist...\n"),
    ("No such table...\n"),
	("Out of memory...\n"),
	("Not in transaction...\n"),
	("Can not remomve the tablespace...\n"),
	("Can not execute backup...\n"),
	("Can not execute restore...\n"),
	("To avoid wraparound data loss, stop the postmaster and use a standalone backend to vacuum...\n"),
	("Check system status failed ...\n"),
	("HA group creator exist...\n"),
	("Replaction init error...\n"),
	("Data corrupted...\n"),
	("Wal receiver fault...\n")
};

//the exception messages for the query engine.
//static const char Error_Message_QueryEngine[][LEN_DIMENSION_0]={
static const char* Error_Message_QueryEngine[]={
    ("It is reserved by system, and do not use it directly...\n"),
    ("Can not open the container...\n"),
    ("Query Interrupted by user...\n"),
	("Query timeout...\n")
};

//the exceptions messages for the query engine.
//static const char Error_Message_Other[][LEN_DIMENSION_0] ={
static const char* Error_Message_Other[]={
    ("It is reserved by system, and do not use it directly...\n"),
    ("The segmentation fault...\n"),                                                        /*ERR_VIOLATE_SEGMENTATION_FAULT*/
    ("Out of boundary...\n"),                                                                /*ERR_VIOLATE_OUT_OF_BOUNDARY*/
    ("Unrecognized command...\n"),                                                          /*ERR_UNKNOWN_COMMAND*/
    ("Feature not supported yet."),                                                        /*ERR_NOT_SUPPORTED_FEATURE*/
    ("\nFailed to allocate %d bytes of memory."),                                           /*ERR_FAILED_TO_ALLOC_MEMORY*/
    ("\nObject are not destroyed. Numbers of objects are not destroyed: %d."),                       /*ERR_OBJECT_NOT_DESTROYED*/  
    ("Out of memory....\n"),                                                                /*ERR_OUT_OF_MEMORY*/
    ("The pointer does not exist...\n"),                                                    /*ERR_NULL_POINTER*/
    ("The string is empty...\n"),                                                           /*ERR_EMPTY_STRING*/
    ("Command syntax is error...\n"),                                                 /*ERR_PARAMETER_NUMBER*/
    ("Unknown exception...\n"),                                                             /*ERR_UNKNOWN_EXCEPTION*/
    ("Conversion failed...\n"),                                                             /*can not do conversion*/
	("File open failed...\n"),                                                              /*ERR_FILE_OPEN_FAILED*/
	("File is empty...\n"),                                                                 /*ERR_FILE_EMPTY*/
	("The uploaded file is too large...\n"),                                                      /*ERR_FILE_TOO_LARGE*/ 
    ("bad key.\n"),
    ("Doc Content erro...\n")
};

//the exception messages for engine execution.
//const static char Error_Message_Array_Execution[][LEN_DIMENSION_0] = {
const static char* Error_Message_Array_Execution[] = {
    ("It is reserved by system, and do not use it directly...\n"),
    ("Unable to open data files...\n"),                                                     /*ERR_OPEN_CONTAINER*/  
    ("Container does not exist...\n"),                                                           /*ERR_NOTEXISTS_CONTAINER*/
    ("Can not add document...\n"),                                                          /*ERR_ADD_DOCUMENT*/
    ("Can not create container...\n"),                                                      /*ERR_CREATE_CONTAINER*/
    ("Query statement can not be executed...\n"),                                                                   /*ERR_QUERY*/
    ("Remove container error...\n"),                                                        /*ERR_REMOVE_CONTAINER*/
    ("Container already exists ...\n"),                                                      /*ERR_CONTAINER_ALREADY_EXISTS*/
    ("Update document error...\n"),                                                         /*ERR_UPDATE_DOCUMENT*/
    ("Remove document error...\n"),                                                         /*ERR_REMOVE_DOCUMENT*/
    ("Can not open container...\n"),                                                        /*ERR_CANNOT_OPEN_CONTAINER*/
    ("Add index error...\n"),                                                               /*ERR_ADD_INDEX*/
    ("Remove index error...\n"),                                                            /*ERR_REMOVE_INDEX*/
    ("Can not execute the prepared statement...\n"),                                        /*ERR_PS_EXECUTE*/
    ("Parameter error...\n"),                                                               /*ERR_PARAMETER_INVALID*/
    ("Can't open the system database...\n"),                                                /*ERR_CAN_NOT_OPEN_SYSTEM_DB*/
    ("Can't access the system database...\n"),                                              /*ERR_CAN_NOT_ACCESS_SYSTEM_DB*/
    ("Database already exists...\n"),                                                        /*ERR_DATABASE_ALREADY_EXIST*/
    ("Database doesn't exist...\n"),                                                        /*ERR_DATABASE_NOT_EXIST*/
    ("User already exists...\n"),                                                            /*ERR_USER_ALREADY_EXIST*/
    ("User doesn't exist...\n"),                                                            /*ERR_USER_NOT_EXIST*/
    ("Role already exists...\n"),                                                            
    ("Role doesn't exist...\n"),                                                            
    ("Can not be deleted from the user table. Failed to find the username...\n"),
    ("Can not delete the system table user, and deletion failed...\n"),
    ("Can not be deleted from the userprivilege table and failed to find the userID ...\n"),
    ("Can not delete the system table userprivilege, and deletion failed...\n"),
    ("Can not be deleted from the roleuser table, and failed to find the userIDuserID fail...\n"),
    ("Can not delete the system table roleuser by user ID, and deletion failed...\n"),
    ("Can not be deleted from the role table, and failed to find the rolename...\n"),
    ("Can not delete the system table role, and deletion failed...\n"),
    ("Can not be deleted from the roleprivilege table, and failed to find the roleID ...\n"),
    ("Can not delete the system table roleprivilege, and deletion failed...\n"),
    ("Can not be deleted from the RoleUser table and failed to find the roleID...\n"), 
    ("Can not delete the system table roleuser by the role ID, and deletion failed...\n"),
    ("Password is incorrect...\n"),                                                         /*ERR_PASSWORD_INCORRECT*/               
    ("Prepared statement doesn't exist...\n"),                                                /*ERR_PREPARED_STATEMENT_NOT_EXIST*/        
    ("Can not access the collection...\n"),                                                   /*ERR_CAN_NOT_ACCESS_COLLECTION*/
    ("Can not access the doc file...\n"),                                                     /*ERR_CAN_NOT_ACCESS_DOCFILE*/ 
    ("Collection already exists...\n"),                                                        /*ERROR_COLLECTION_ALREADY_EXIST*/
    ("Collection does not exist...\n"),                                                       /*ERROR_COLLECTION_NOT_EXIST */
    ("Doc file already exists...\n"),                                                         /*ERROR_DOCFILE_ALREADY_EXIST*/
    ("Doc file does not exist...\n"),                                                         /*ERROR_DOCFILE_NOT_EXIST*/
    ("Can not delete system table...\n"),                                                     /*ERROR_CAN_NOT_DELETE_SYSTEM_TABLE*/
    ("Can not insert system table...\n"),                                                     /*ERROR_CAN_NOT_INSERT_SYSTEM_TABLE*/
    ("Can not delete system tree table...\n"),                                                /*ERROR_CAN_NOT_DELETE_SYSTEM_TREE_TABLE*/
    ("Can not insert system tree table...\n"),                                                /*ERROR_CAN_NOT_INSERT_SYSTEM_TREE_TABLE*/
    ("Relationship does not exist...\n"),                                                      /*ERROR_RELATIONSHIP_NOT_EXIST*/
    ("User or Role does not exist...\n") ,
    ("The system privilege is not granted...\n"),
    ("The object privilege is not granted...\n"),
    ("collection is a mirror collection, operation prohibited...\n"),            
    ("Can not check the cascade, and it is not found...\n"),
    ("The docitem is cascade, and deletion error occured...\n"),
    ("The current database does not exist...\n"),
    ("The current user does not exist...\n"),
    ("the relationship error, bind fail...\n"),
    ("The relationship has existed. Relationship error...\n"),
    ("The privilege does not exist...\n"),
    ("The user is not admin...\n"),          
    ("The path does not exist..."),             
    ("The resource is not found...\n"),
    ("The user is admin,you can not change it...\n"),
    ("The role is sysadmin,you can not change it...\n"),
    ("The syntax of name is wrong, and you should begin with letters or underline ...\n"), 
    ("Internal Query Exceptions Occurred...\n"),
    ("The name is illegal...\n"),
    ("Database is used...\n"),
    ("A tablespace can not be created in the path\n"),
    ("The table space has already existed...\n"),
    ("Slave machine should be read only...\n"),
    ("Large object does not exist...\n"),                                                     /*ERR_NOTEXISTS_LARGE_OBJECT*/
    ("TableSpace does not exist...\n"),
    ("The object has existed...\n"),                                                         /*ERR_TABLE_SPACE_ALREADYEXIST*/
    ("Truncate Container error...\n"),                                                       /*ERR_TRUNCATE_INDEX*/
    ("Rename container error...\n"),
    ("Path has already existed...\n"),
    ("Dump container error...\n"),
    ("Load container error...\n"),
    ("The Object does not exist...\n"),
    ("Container does not commit...\n"),
    ("The Overflow Exceptions Occurred...\n"),
    ("New password is same with old password...\n"),
    (" LagerObject erro...\n"),
    ("Container has full .....\n"),
    ("Grant node erro...\n"),
    ("List node priv erro...\n"),
    ("Revoke node privilege error...\n"),
    ("Check node privilege error...\n"),
    ("Sql database exception...\n") ,                                                                    /*ERR_SQL_DATABASE*/
    ("ReIndex exception...\n"),         //ERR_RE_INDEX
    ("Index does not exist...\n"),    //ERR_NO_PATHINDEX
    ("Index has existed...\n"),       //ERR_HAVE_PATHINDEX 
    ("Index Meta-data Inserting/Updating failed...\n"),//ERR_PATHINDEX_FAIL
    ("Index Meta-data has existed...\n"),//ERR_PATHINDEX_REPEAT 
    ("Can not delete the database...\n")  ,
    ("User or Role is not admin...\n"),
    ("Sequence Error...\n")            //ERR_SEQUENCE_ERROR                                                    
} ;

//the exceptions messages for the fulltext.
//static const char Error_Message_Array_FullText[][LEN_DIMENSION_0] ={
static const char* Error_Message_Array_FullText[]={
    ("Reserved By system, Do not use it directly...\n"),
    ("The fulltext resource is not found...\n"),                                                                   /*ERR_RESOURCE_NOT_FOUND_FULLTEXT*/
    ("Can not find stopword in stopword lists...\n"),                                                             /*ERR_STOPWORD_NOTFOUND*/
    ("An expression violates the restriction of using FTWindow and FTDistance...\n"),                            /*ERR_FTWINDOW_AND_FTDISTANCE*/
    ("Fulltext query contains FTContent operator...\n"),                                                           /*ERR_QUERY_CONTAINS_FTCONTENT*/
    ("A single language uses FTLanguageOptions, encounters more than one distinct language option...\n"),          /*ERR_FTLANGUAGE_OPTIONS*/
    ("FTCaseOptions encounters the lowercase or uppercase option...\n" ),                                          /*ERR_FTCASE_OPTION_EXCEPTION*/
    ("Weight value is out of the required range of values...\n"),                                                  /*ERR_WEIGHT_VALUE*/
    ("One of mild not selection operands evaluated to an ALLMatches that contains a StringExclude...\n"),          /*ERR_MILD_NOT_SELECTION*/
    ("There is more than one match option of any given match option group within a single FTMatchOptions...\n"),   /*ERR_FTMATCH_OPTIONS*/
    ("Query string violates wildcard syntax...\n"),                                                                /*ERR_WILDCARD_SYNTAX*/
    ("IKAnalyzer error..."),                                                                                       /*ERR_IKANALYZER*/
    ("Fulltext resource operations, add, update or delete, can not be executed properly..."),                      /*ERR_FULLTEXT_RES_OPERATION*/
    ("Can not load dicitonary...")                                                                                 /*ERR_USE_DICTIONARY*/
};

static const char* Error_Message_KVTable[]={
    ("It is reserved by system, and do not use it directly...\n"),
    ("kv_table already exist...\n"),
    ("kv_table has not been created...\n"),
    ("kv_index_table already exist...\n"),
    ("kv_index_table has not been created...\n"),
    ("kv_table dump failed...\n"),
    ("kv_table load failed...\n"),
	("kv_table index is already 'unique'...\n")
};

static const char* Error_Message_RollBack[]={
    ("It is reserved by system, and do not use it directly...\n"),
    ("Server will abort the current Transaction..."),
    ("Module is duplicate,transaction will rollBack...\n"),
    ("Update concurrently,transaction will rollBack...\n"),
    ("Deadlock is detected,transaction will rollBack...\n"),
    ("Serializable check failed, this transaction has been aborted, please try again...\n")
};

static const char*	Error_Message_GraphEngine[]={
	("It is reserved by system, and do not use it directly...\n"),
	("Out of memory...\n"),
	("Graph has not been created...\n"),
	("The current graph does not exist...\n"),
	("Add graph triple failed...\n"),
	("Delete graph triple failed...\n"),
	("Update graph triple failed...\n"),
	("Search graph triple failed...\n"),
	("Drop graph failed...\n"),
	("Parse file does not exist...\n"),
	("Reuslt type can not be None...\n"),
	("Execution failed...\n")
};

///THE RE-DEFINED THE BASIC DATA TYPE TO MEANINGFUL DESCRIPTIONS
//typedef int32 int32_base_t ; 
//typedef int32_base_t ErrorCode_t ;
class FounderXDB_EXPORT ExceptionsThrow {
public:
    static void ThrowExceptions (ErrorCode_t errorcode) ; 
    static void ThrowExceptions (ErrorCode_t errorcode, const char* msg) ;
};

//the base exception class. DB,NET,Other exceptions are derived from this class.    
class FounderXDB_EXPORT XdbBaseException : public virtual std::exception
{
protected:
    const static uint32 EXCEPTION_INFO_BUFLEN = 512*2;
    ErrorCode_t m_errorCode;
    // Don't use std::string here, otherwise it's likely to ////throw another exception 
    // while handling the exception.
    char m_infobuf[EXCEPTION_INFO_BUFLEN+1];

    explicit XdbBaseException(const XdbBaseException& obj) ;
    const XdbBaseException& operator= (const XdbBaseException& obj) ;
    ///////////////////////////////////////////////////////////////////////////////////
private:
    ///TO PREVENT COLLIDING WITH SYSTEM ERROR CODES, WE DEFINE A BASEPOINT. 
    ///ALL OF THE EXCEPTION CODES ARE STARTED FROM THIS BASEPOINT. JUST ONLY 
    ///ADJUST THE EXCEPTION CODE VALUE VIA CHANGING THE BASEPOINT VALUE.
    static const int32 ERR_BASE_POINT_NET = - 1000;
    static const int32 ERR_BASE_POINT_STORAGENGINE = -2000;
    static const int32 ERR_BASE_POINT_QUERYENGINE = -3000;
    static const int32 ERR_BASE_POINT_DB = -4000;
    static const int32 ERR_BASE_POINT_OTHER = -5000 ;
    static const int32 ERR_EXECUTION_ENGINE_BASE = -6000;
    static const int32 ERR_BASE_POINT_FULLTEXT = -7000;
    static const int32 ERR_BASE_POINT_KVTABLE = -8000;
    static const int32 ERR_BASE_POINT_ROLLBACK = -9000;
	static const int32 ERR_BASE_POINT_GRAPHENGINE = -10000;
    static const int32 ERR_BASE_POINT_LIMIT = -11000;
    
public:
    static int32 getUpperErrorCode () {
        return XdbBaseException::ERR_BASE_POINT_NET;
    } 
    static int32 getLowerErrorCode () {
        return XdbBaseException::ERR_BASE_POINT_LIMIT ;
    }
    ///THE FOLLOWING ERROR CODE IS A EXAMPLE CODE FOR DEMONSTRATION.
    ///THE OPERATION SUCCEED.
    const static int32 ERR_OPERATION_SUCCEED =0 ; 
    ///**************************************************************************/

	const static int32 ERR_STORAGE_DEADLOCK_DECTED = ERR_BASE_POINT_STORAGENGINE + -2;
	const static int32 ERR_STORAGE_SERIALIZATION_FAILURE = ERR_BASE_POINT_STORAGENGINE + -3;
	const static int32 ERR_STORAGE_OBJECT_IN_USE = ERR_BASE_POINT_STORAGENGINE + -4;
	const static int32 ERR_STORAGE_TUPLE_UPDATE_CONCURRENT = ERR_BASE_POINT_STORAGENGINE + -5;
	const static int32 ERR_STORAGE_TABLE_SPACE_ALREADY_EXIST = ERR_BASE_POINT_STORAGENGINE + -6;
	const static int32 ERR_STORAGE_STANDBY_TRY_TO_UPDATE = ERR_BASE_POINT_STORAGENGINE + -7;
	const static int32 ERR_STORAGE_TWO_MASTERS = ERR_BASE_POINT_STORAGENGINE + -8;
	const static int32 ERR_STORAGE_UNIQUE_VIOLATION = ERR_BASE_POINT_STORAGENGINE + -9;
	const static int32 ERR_STORAGE_OBJECT_NOT_EXIST = ERR_BASE_POINT_STORAGENGINE + -10;
    const static int32 ERR_STORAGE_TABLE_NOT_EXIST = ERR_BASE_POINT_STORAGENGINE + -11;
    const static int32 ERR_STORAGE_OUT_OF_MEMORY = ERR_BASE_POINT_STORAGENGINE + -12;
	const static int32 ERR_STORAGE_NOT_IN_TRANSACTION = ERR_BASE_POINT_STORAGENGINE + -13;
    const static int32 ERR_STORAGE_TABLESPACE_REMOVE = ERR_BASE_POINT_STORAGENGINE + -14;
	const static int32 ERR_STORAGE_EXECUTE_BACKUP = ERR_BASE_POINT_STORAGENGINE + -15;
	const static int32 ERR_STORAGE_EXECUTE_RESTORE = ERR_BASE_POINT_STORAGENGINE + -16;
	const static int32 ERR_STORAGE_PROGRAM_LIMIT_EXCEEDED = ERR_BASE_POINT_STORAGENGINE + -17;
	const static int32 ERR_STORAGE_SYSTEM_STATUS = ERR_BASE_POINT_STORAGENGINE + -18;
	const static int32 ERR_STORAGE_GROUP_CREATOR_EXIST = ERR_BASE_POINT_STORAGENGINE + -19;
	const static int32 ERR_STORAGE_REPLICATION_INIT = ERR_BASE_POINT_STORAGENGINE + -20;
	const static int32 ERR_STORAGE_DATA_CORRUPTED = ERR_BASE_POINT_STORAGENGINE + -21;
	const static int32 ERR_STORAGE_WAL_RECEIVER = ERR_BASE_POINT_STORAGENGINE + -22;
    ///INVALID USER FOR CONNECTING TO A DB.
    const static int32 ERR_DB_INVALIDE_USER = ERR_BASE_POINT_DB + -1 ; 

    ///INVALID PASSWORD FOR CONNECTION TO A DB. 
    const static int32 ERR_DB_INVALIDE_PASSWORD = ERR_BASE_POINT_DB + -2 ; 

    ///THE STATEMENT YOU TYPED IS INVALID, SUCH AS GRAMMAR ERRORA, ETC.
    const static int32 ERR_DB_INVALIDE_STATEMENT = ERR_BASE_POINT_DB + -3 ; 

    ///THE USER YOU CREATED ALREADY EXISTS.
    const static int32 ERR_DB_USER_EXISTS = ERR_BASE_POINT_DB + -4 ; 

    ///THE USER YOU CREATED DOES NOT EXIST.
    const static int32 ERR_DB_USER_NOT_EXISTS = ERR_BASE_POINT_DB + -5 ; 
    
    ///THE ROLE ALREADY EXISTS. 
    const static int32 ERR_DB_ROLE_EXISTS = ERR_BASE_POINT_DB + -6 ; 
    
    ///THE ROLE ALREADY NOT EXIST. 
    const static int32 ERR_DB_ROLE_NOT_EXISTS = ERR_BASE_POINT_DB + -7 ; 
    
    ///NOT PERMISSION TO PERFORM THIS OPERATION. 
    const static int32 ERR_DB_NOT_AUTHORIZED = ERR_BASE_POINT_DB + -8 ; 
    
    const static int32 ERR_DATABASE_NOT_USED  = ERR_BASE_POINT_DB + -9;
    ///**************************************************************************/
    
    ///INVALID IP ADDRESS PROVIDED.
    const static int32 ERR_NET_INVALIDE_IP = ERR_BASE_POINT_NET + -1 ; 

    ///THE SERVICE DOES NOT OPEN.
    const static int32 ERR_NET_SERVER_NOT_OPEN = ERR_BASE_POINT_NET + -2 ; 

    ///CAN NOT TO CONNECT TO A SERVER, TIMEOUT ERROR OCCURS.
    const static int32 ERR_NET_TIMEOUT = ERR_BASE_POINT_NET + -3 ; 

    ///CERTIFICATES ARE INVALID.
    const static int32 ERR_NET_INVALID_CERTIFICATES = ERR_BASE_POINT_NET + -4 ; 
    
    ///UNKNOWN COMMAND (unreconginzed command). 
    const static int32 ERR_NET_UNKNOWNCMMD = ERR_BASE_POINT_NET + -5 ; 
    
    ///CONNECTION CLOSED
    const static int32 ERR_NET_CONNECTION_CLOSED  = ERR_BASE_POINT_NET + -6;
    
    ///NO TRANSACTION
    const static int32 ERR_NET_NO_TRANSACTION = ERR_BASE_POINT_NET + -7;
    
    ///EXECUTOR REQUEST CREATE FAILED
    const static int32 ERR_NET_EXECUTOR_REQUEST_CREATE_FAILED = ERR_BASE_POINT_NET + -8;
    
    ///NOT LOGIN
    const static int32 ERR_NET_NOT_LOGIN = ERR_BASE_POINT_NET + -9;
    
    ///SOCKET EXCEPTION
    const static int32 ERR_SOCKET_EXCEPTION = ERR_BASE_POINT_NET + -10;

    ///address is in use
    const static int32 ERR_NET_ADDRESS_IN_USE = ERR_BASE_POINT_NET + -11;
    
    ///transaction also exist
    const static int32 ERR_NET_TRANSACTION_EXIST = ERR_BASE_POINT_NET + -12;

    /// data node machine is down
    const static int32 ERR_NET_DATANODE_DOWN = ERR_BASE_POINT_NET + -13;
    
    const static int32 ERR_NET_SSL = ERR_BASE_POINT_NET + -14;

    ///recursive login. 
    const static int32 ERR_NET_RECURSIVE_LOGIN = ERR_BASE_POINT_NET + -15;

	///uncorrect serser config
	const static int32 ERR_NET_SVRCFG_FILE = ERR_BASE_POINT_NET + -16;

	// cluster environment not readby for client.
	const static int32 ERR_NET_CLUSTER_NOT_READY = ERR_BASE_POINT_NET + -17;

    ///**************************************************************************/
    
    ///SEGMENTATION_FAULT OCCUR
    const static int32 ERR_VIOLATE_SEGMENTATION_FAULT = ERR_BASE_POINT_OTHER + -1 ; 
    
    ///ACCESSES AN INVALID INDEX, OUT OF BOUNDARY. 
    const static int32 ERR_VIOLATE_OUT_OF_BOUNDARY = ERR_BASE_POINT_OTHER + -2 ;
    
    ///THE COMMAND YOU TYPED IS NOT RECOGNIZED.
    const static int32 ERR_UNKNOWN_COMMAND = ERR_BASE_POINT_OTHER + -3 ;   

    ///THE FEATURE NOT SUPPORT
    const static int32 ERR_NOT_SUPPORTED_FEATURE = ERR_BASE_POINT_OTHER + -4;

    ///THE HEAP/STACK PERHAPS BE USED UP, OR SYSTEM ERRORS OCCUR.
    const static int32 ERR_FAILED_TO_ALLOC_MEMORY = ERR_BASE_POINT_OTHER + -5;

    ///THE ALLOCATED OBJECTS CAN NOT BE RELEASED.
    const static int32 ERR_OBJECT_NOT_DESTROYED = ERR_BASE_POINT_OTHER + -6;
    
    ///THE ALLOCATED OBJECTS CAN NOT BE RELEASED.
    const static int32 ERR_OUT_OF_MEMORY = ERR_BASE_POINT_OTHER + -7;
    
    ///THE POINTER NOT EXIST.
    const static int32 ERR_NULL_POINTER = ERR_BASE_POINT_OTHER + -8;

    ///EMPTY STRING.
    const static int32 ERR_EMPTY_STRING = ERR_BASE_POINT_OTHER + -9;
    
    ///PARAMETER NUMBER ERROR.
    const static int32 ERR_COMMAND_SYNTAX = ERR_BASE_POINT_OTHER + -10;
    
    ///UNKNOWN EXCEPTION.
    const static int32 ERR_UNKNOWN_EXCEPTION = ERR_BASE_POINT_OTHER + -11;
    
    //CONVERSION FAILED.
    const static int32 ERR_CONVERSION_FAILED = ERR_BASE_POINT_OTHER + -12;
	
	// FILE OPEN FILED
	const static int32 ERR_FILE_OPEN_FAILED = ERR_BASE_POINT_OTHER + -13;

	// THERE IS NOTHING IN THE FILE
	const static int32 ERR_FILE_EMPTY = ERR_BASE_POINT_OTHER + -14;

	// THERE IS NOTHING IN THE FILE
	const static int32 ERR_FILE_TOO_LARGE = ERR_BASE_POINT_OTHER + -15;
    //BAD encrypt/decrypt key.
    const static int32 ERR_BAD_KEY = ERR_BASE_POINT_OTHER + -16;
    const static int32 ERR_DOC_CONTENT = ERR_BASE_POINT_OTHER + -17;
    ///**************************************************************************/
    
    ///CAN NOT OPEN CONTAINER FILE.
    const static int32 ERR_OPEN_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -1; 
    
    ///NOT EXIST CONTAINER. 
    const static int32 ERR_NOTEXISTS_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -2; 

    ///CAN NOT ADD A DOCUMENT.
    const static int32 ERR_ADD_DOCUMENT = ERR_EXECUTION_ENGINE_BASE + -3; 

    ///CAN NOT CREATE A CONTAINER.
    const static int32 ERR_CREATE_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -4; 
    
    ///QUERY STATEMENT ERROR.
    const static int32 ERR_QUERY = ERR_EXECUTION_ENGINE_BASE + -5; 
    
    ///REMOTE CONTAINER ERROR
    const static int32  ERR_REMOVE_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -6; 
    
    ///CONTAINER ALREADY EXISTS
    const static int32 ERR_CONTAINER_ALREADY_EXISTS = ERR_EXECUTION_ENGINE_BASE + -7; 
    
    ///Update document error.
    const static int32 ERR_UPDATE_DOCUMENT = ERR_EXECUTION_ENGINE_BASE + -8; 

    ///Remove document error
    const static int32 ERR_REMOVE_DOCUMENT = ERR_EXECUTION_ENGINE_BASE + -9; 
    
    ///Can not open container
    const static int32 ERR_CANNOT_OPEN_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -10; 
    
    ///ADD INDEX ERROR
    const static int32 ERR_ADD_INDEX = ERR_EXECUTION_ENGINE_BASE + -11;

    ///REMOVE INDEX ERROR
    const static int32 ERR_REMOVE_INDEX = ERR_EXECUTION_ENGINE_BASE + -12;

    ///PREPARESTATEMENT EXECUTION  ERROR
    const static int32 ERR_PS_EXECUTE = ERR_EXECUTION_ENGINE_BASE + -13;

    ///PARAMETER INVALID
    const static int32 ERR_PARAMETER_INVALID = ERR_EXECUTION_ENGINE_BASE + -14;

    ///CAN NOT OPEN THE SYSTEM DATABASE
    const static int32 ERR_CAN_NOT_OPEN_SYSTEM_DB = ERR_EXECUTION_ENGINE_BASE + -15;

    ///CAN NOT ACCESS THE SYSTEM DATABASE
    const static int32 ERR_CAN_NOT_ACCESS_SYSTEM_DB = ERR_EXECUTION_ENGINE_BASE + -16;

    ///DATABASE ALREADY EXIST
    const static int32 ERR_DATABASE_ALREADY_EXIST = ERR_EXECUTION_ENGINE_BASE + -17;
    
    ///DATABASE NOT EXIST
    const static int32 ERR_DATABASE_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -18;
    
    /// USER ALREADY EXIST
    const static int32 ERR_USER_ALREADY_EXIST = ERR_EXECUTION_ENGINE_BASE + -19;
    
    /// USER  NOT EXIST
    const static int32 ERR_USER_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -20;
    
    ///THE ROLE ALREADY EXISTS. 
    const static int32 ERROR_ROLE_ALREADY_EXIST = ERR_EXECUTION_ENGINE_BASE + -21 ; 

    ///ROLE  NOT EXIST
    const static int32 ERROR_ROLE_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -22;

    ///CAN NOT BIND THE USER NAME
    const static int32 ERROR_CAN_NOT_BIND_USER_NAME = ERR_EXECUTION_ENGINE_BASE + -23;

    ///CAN NOT DELETE THE USER TABLE
    const static int32 ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER = ERR_EXECUTION_ENGINE_BASE + -24;

    ///CAN NOT BIND THE USER ID
    const static int32 ERROR_CAN_NOT_BIND_USER_ID = ERR_EXECUTION_ENGINE_BASE + -25;

    ///CAN NOT DELETE THE USER PRIVILEGE TABLE
    const static int32 ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER_PRIVILEGE = ERR_EXECUTION_ENGINE_BASE + -26;

    ///CAN NOT BIND USER ID FROM ROLE USER
    const static int32 ERROR_CAN_NOT_BIND_USER_ID_FROM_ROLE_USER = ERR_EXECUTION_ENGINE_BASE + -27;

    ///CAN NOT DELETE THE ROLE USER TABLE BY USER ID
    const static int32 ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_USER_ID = ERR_EXECUTION_ENGINE_BASE + -28;

    ///CAN NOT BIND THE ROLE NAME
    const static int32 ERROR_CAN_NOT_BIND_ROLE_NAME = ERR_EXECUTION_ENGINE_BASE + -29;

    ///CAN NOT DELETE TABLE ROLE
    const static int32 ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE = ERR_EXECUTION_ENGINE_BASE + -30;

    ///CAN NOT BIND THE ROLE ID
    const static int32 ERROR_CAN_NOT_BIND_ROLE_ID = ERR_EXECUTION_ENGINE_BASE + -31;

    ///CAN NOT DELETE TABLE ROLE PRIVILEGE
    const static int32 ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE_PRIVILEGE = ERR_EXECUTION_ENGINE_BASE + -32;

    ///CAN NOT BIND THE USER ID FROM ROLE USER
    const static int32 ERROR_CAN_NOT_BIND_ROLE_ID_FROM_ROLE_USER = ERR_EXECUTION_ENGINE_BASE + -33;

    ///CAN NOT DELETE TABLE ROLE USER BY ROLE ID
    const static int32 ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_ROLE_ID = ERR_EXECUTION_ENGINE_BASE + -34;
    
    ///PASSWORD IS INCORRECT
    const static int32 ERR_PASSWORD_INCORRECT = ERR_EXECUTION_ENGINE_BASE + -35;
    
    ///PREPARED STATEMENT DOESN'T EXIST
    const static int32 ERR_PREPARED_STATEMENT_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -36;
    
    ///CAN NOT ACCESS THE COLLECTION
    const static int32 ERR_CAN_NOT_ACCESS_COLLECTION = ERR_EXECUTION_ENGINE_BASE + -37;
    
    ///CAN NOT ACCESS THE DOCFILE
    const static int32 ERR_CAN_NOT_ACCESS_DOCFILE = ERR_EXECUTION_ENGINE_BASE + -38;
    
    ///THE COLLECTION ALREADY EXISTS. 
    const static int32 ERROR_COLLECTION_ALREADY_EXIST = ERR_EXECUTION_ENGINE_BASE + -39; 
    ///THE COLLECTION NOT EXISTS. 
    const static int32 ERROR_COLLECTION_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -40; 
    ///THE DOCFILE ALREADY EXISTS. 
    const static int32 ERROR_DOCFILE_ALREADY_EXIST = ERR_EXECUTION_ENGINE_BASE + -41; 
    ///THE DOCFILE NOT EXISTS. 
    const static int32 ERROR_DOCFILE_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -42; 
    
    ///COT NOT DELETE THE SYSTEMTABLE
    const static int32 ERROR_CAN_NOT_DELETE_SYSTEM_TABLE = ERR_EXECUTION_ENGINE_BASE + -43; 
    
    ///COT NOT INSERT THE SYSTEMTABLE
    const static int32 ERROR_CAN_NOT_INSERT_SYSTEM_TABLE = ERR_EXECUTION_ENGINE_BASE + -44; 
    
    ///COT NOT DELETE THE SYSTEMTREETABLE
    const static int32 ERROR_CAN_NOT_DELETE_SYSTEM_TREE_TABLE = ERR_EXECUTION_ENGINE_BASE + -45; 
    
    ///COT NOT INSERT THE SYSTEMTREETABLE
    const static int32 ERROR_CAN_NOT_INSERT_SYSTEM_TREE_TABLE = ERR_EXECUTION_ENGINE_BASE + -46;

    ///RELATIONSHIP NOT EXIST
    const static int32 ERROR_RELATIONSHIP_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -47;

    ///USER OR ROLE DOESN'T EXIST
    const static int32 ERROR_USER_OR_ROLE_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -48;
    
    ///SYSTEM PRIVILEGE NOT GRANTED
    const static int32 ERROR_SYS_PRIVILEGE_NOT_GRANTED = ERR_EXECUTION_ENGINE_BASE + -49;
    
    ///OBJECT PRIVILEGE NOT GRANTED
    const static int32 ERROR_OBJECT_PRIVILEGE_NOT_GRANTED = ERR_EXECUTION_ENGINE_BASE + -50;
    ///COLLECTION IS MIRROR
    const static int32 ERROR_DELETE_COLLECTION = ERR_EXECUTION_ENGINE_BASE + -51;
    ///THE CHECK DOCITEM CASCADE EXISTS. 
    const static int32 ERROR_CHECK_DOCITEM_CASCADE = ERR_EXECUTION_ENGINE_BASE + -52; 
    ///THE DELETE DOCFILE CASCADE. 
    const static int32 ERROR_DELETE_DOCITEM_CASCADE = ERR_EXECUTION_ENGINE_BASE + -53;
    ///GET THE CURRENTDATABASE.
    const static int32 ERROR_GET_CURRENT_DATABASE = ERR_EXECUTION_ENGINE_BASE + -54;
    ///GET THE CURRENTUSER.
    const static int32 ERROR_GET_CURRENT_USER = ERR_EXECUTION_ENGINE_BASE + -55;

    ///CHECK ITEN RALATION.
    const static int32 ERROR_CHECK_ITEM_RELATION = ERR_EXECUTION_ENGINE_BASE + -56;
    ///THE DOCITEM RELATIONSHIP HAS EXIST
    const static int32 ERROR_DOCITEM_RELATION_HAS_EXIST = ERR_EXECUTION_ENGINE_BASE + -57; 
    
    const static int32 ERROR_PRIVILEGE_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -58;
    
    const static int32 ERROR_USER_NOT_ADMIN = ERR_EXECUTION_ENGINE_BASE + -59;
    
    const static int32 ERROR_PATH_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -60;

    const static int32 ERR_RESOURCE_NOT_FOUND = ERR_EXECUTION_ENGINE_BASE + -61;
    
    const static int32 ERROR_CAN_NOT_CHANGE_ADMIN_USER = ERR_EXECUTION_ENGINE_BASE + -62;
    
    const static int32 ERROR_CAN_NOT_CHANGE_SYS_ADMIN_ROLE = ERR_EXECUTION_ENGINE_BASE + -63;
    
    const static int32 ERR_DATABASE_NAMED_SYNTAX = ERR_EXECUTION_ENGINE_BASE + -64;
    
    const static int32 ERR_INTERNAL_QUERY = ERR_EXECUTION_ENGINE_BASE + -65;
    
    const static int32 ERR_NAME_LEGAL = ERR_EXECUTION_ENGINE_BASE + -66;
    
    const static int32 ERR_DATABASE_USED = ERR_EXECUTION_ENGINE_BASE + -67;
 
    const static int32 ERR_CREATE_TABLE_SPACE = ERR_EXECUTION_ENGINE_BASE + -68;   
    
    const static int32 ERR_TABLE_SPACE_ALREADYEXIST = ERR_EXECUTION_ENGINE_BASE + -69; 
    
    const static int32 ERR_SLAVE_READ_ONLY = ERR_EXECUTION_ENGINE_BASE + -70; 

    const static int32 ERR_NOTEXISTS_LARGE_OBJECT = ERR_EXECUTION_ENGINE_BASE + -71; 
    
    const static int32 ERR_TABLESPACE_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -72; 

    const static int32 ERR_OBJECT_ALREADY_EXIST = ERR_EXECUTION_ENGINE_BASE + -73; 
    
    const static int32 ERR_TRUNCATE_INDEX = ERR_EXECUTION_ENGINE_BASE + -74;
    
    const static int32 ERR_RENAME_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -75;
    
    const static int32 ERROR_PATH_ALREADY_EXIST = ERR_EXECUTION_ENGINE_BASE + -76;
    
    const static int32 ERR_DUMP_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -77; 
    
    const static int32 ERR_LOAD_CONTAINER = ERR_EXECUTION_ENGINE_BASE + -78; 
    
    const static int32 ERR_OBJECT_NOT_EXIST = ERR_EXECUTION_ENGINE_BASE + -79;
    
    const static int32 ERR_CONTAINER_NOT_COMMIT = ERR_EXECUTION_ENGINE_BASE + -80; //container not commit.

	const static int32 ERR_ID_OVERFLOW = ERR_EXECUTION_ENGINE_BASE + - 81;

	const static int32 ERR_NEW_PASSWORD_SAME_WITH_OLD_PASSWORD = ERR_EXECUTION_ENGINE_BASE + - 82;
	
    ///CONTAINER ID  OUT
    const static int32 ERR_CONTAINER_FULL = ERR_EXECUTION_ENGINE_BASE + -83; 
	const static int32 ERR_LARGE_OBJECT = ERR_EXECUTION_ENGINE_BASE + -84;
    
    ///GRANT NODE ERRO
    const static int32 ERR_GRANT_NODE = ERR_EXECUTION_ENGINE_BASE + -85;
    
    const static int32 ERR_LIST_PRIV_NODE = ERR_EXECUTION_ENGINE_BASE + -86; 

  	const static int32 ERR_REVOKE_NODE = ERR_EXECUTION_ENGINE_BASE + -87;
  	
  	const static int32 ERR_CHECK_NODE_PRIV = ERR_EXECUTION_ENGINE_BASE + -88;
  	
  	/// SQL database exception.
  	const static int32 ERR_SQL_DATABASE = ERR_EXECUTION_ENGINE_BASE + -89;
  	/// reindex erro 
  	const static int32 ERR_RE_INDEX = ERR_EXECUTION_ENGINE_BASE + -90;
  	
  	/// NoPathIndex erro
  	const static int32 ERR_NO_PATHINDEX = ERR_EXECUTION_ENGINE_BASE + -91;
  	
  	/// HavePathIndex erro
  	const static int32 ERR_HAVE_PATHINDEX = ERR_EXECUTION_ENGINE_BASE + -92;
  	
  	/// PathIndexFail erro
  	const static int32 ERR_PATHINDEX_FAIL = ERR_EXECUTION_ENGINE_BASE + -93;
  	
  	/// pathIndexRepeat erro
  	const static int32 ERR_PATHINDEX_REPEAT = ERR_EXECUTION_ENGINE_BASE + -94;
  	
    const static int32 ERR_DATABASE_DELETE_FAIL = ERR_EXECUTION_ENGINE_BASE + -95;
    
    const static int32 ERR_USEROLE_IS_NOT_ADMIN = ERR_EXECUTION_ENGINE_BASE + -96;
    
    ///sequence erro
    
    const static int32 ERR_SEQUENCE_ERROR = ERR_EXECUTION_ENGINE_BASE + -97;
    

    
    ///**************************************************************************/
    
    ///GO THROUGHT ERR. (FOR SWITCH OR FOR, WHILE, etc.)
    const static int32 ERR_OPERATE_GO_THROUGH_DEFAULT = ERR_BASE_POINT_OTHER + -1024 ;
    ///**************************************************************************/
    
    ///********************************************************************************************/
    //ERR_BASE_POINT_FULLTEXT START
    const static int32 ERR_RESOURCE_NOT_FOUND_FULLTEXT = ERR_BASE_POINT_FULLTEXT + -1;  /*ERR_RESOURCE_NOT_FOUND_FULLTEXT*/
    
    const static int32 ERR_STOPWORD_NOTFOUND = ERR_BASE_POINT_FULLTEXT + -2;         /*err:FTST0008*/
    
    const static int32 ERR_FTWINDOW_AND_FTDISTANCE = ERR_BASE_POINT_FULLTEXT + -3;   /*err:FTST0011*/
    
    const static int32 ERR_QUERY_CONTAINS_FTCONTENT = ERR_BASE_POINT_FULLTEXT + -4;  /*err:FTST0012*/
    
    const static int32 ERR_FTLANGUAGE_OPTIONS = ERR_BASE_POINT_FULLTEXT + -5;        /*err:FTST0013*/
    
    const static int32 ERR_FTCASE_OPTION_EXCEPTION = ERR_BASE_POINT_FULLTEXT + -6;   /*err:FTST0015*/
    
    const static int32 ERR_WEIGHT_VALUE = ERR_BASE_POINT_FULLTEXT + -7;              /*err:FTST0016*/
    
    const static int32 ERR_MILD_NOT_SELECTION = ERR_BASE_POINT_FULLTEXT + -8;        /*err:FTST0017*/
    
    const static int32 ERR_FTMATCH_OPTIONS = ERR_BASE_POINT_FULLTEXT + -9;           /*err:FTST0019*/
    
    const static int32 ERR_WILDCARD_SYNTAX = ERR_BASE_POINT_FULLTEXT + -10;           /*err:FTST0020*/
    
    const static int32 ERR_IKANALYZER = ERR_BASE_POINT_FULLTEXT + -11;               /*IKAnalyzer error*/
    
    const static int32 ERR_FULLTEXT_RES_OPERATION = ERR_BASE_POINT_FULLTEXT + -12;   /*fulltext resourse error*/
    
    const static int32 ERR_USE_DICTIONARY = ERR_BASE_POINT_FULLTEXT + -13;           /*use dictionary error*/
    
    ///THE STATIC FUNCTION, USED FOR GETTING THE CORRESPONDING ERROR MESSAGE.

    // ERR_BASE_POINT_KVTABLE start
    const static int32 ERR_KV_TABLE_EXIST = ERR_BASE_POINT_KVTABLE + -1;               /*kvtable already exist*/
    const static int32 ERR_KV_TABLE_NOT_EXIST = ERR_BASE_POINT_KVTABLE + -2;           /*kvtable does not exist*/
    const static int32 ERR_KV_INDEX_TABLE_EXIST = ERR_BASE_POINT_KVTABLE + -3;         /*kvindextable already exist*/
    const static int32 ERR_KV_INDEX_TABLE_NOT_EXIST = ERR_BASE_POINT_KVTABLE + -4;     /*kvindextable does not exist*/
    const static int32 ERR_KV_TABLE_DUMP_FAILED = ERR_BASE_POINT_KVTABLE + -5;         /*kvindextable dump failed*/
    const static int32 ERR_KV_TABLE_LOAD_FAILED = ERR_BASE_POINT_KVTABLE + -6;         /*kvindextable load failed*/
	const static int32 ERR_KV_TABLE_INDEX_UNIQUE = ERR_BASE_POINT_KVTABLE + -7;         /*kvindextable is unique now*/

	// ERR_BASE_POINT_GRAPHENGINE start
	const static int32 ERR_GRAPH_OUTOFMEMORY = ERR_BASE_POINT_GRAPHENGINE + -1;           		/*Out of memory */
	const static int32 ERR_GRAPH_GRAPH_MODEL_NOT_EXIST = ERR_BASE_POINT_GRAPHENGINE + -2;  		/*Graph  has not been created */
	const static int32 ERR_GRAPH_CURRENT_GRAPH_NOT_EXIST = ERR_BASE_POINT_GRAPHENGINE + -3; 	/*The current graph does not exist.*/
	const static int32 ERR_GRAPH_ADD_TRIPLE = ERR_BASE_POINT_GRAPHENGINE + -4;  				/*add graph triple failed.*/
	const static int32 ERR_GRAPH_DELETE_TRIPLE = ERR_BASE_POINT_GRAPHENGINE + -5;  				/*delete graph triple failed.*/
	const static int32 ERR_GRAPH_UPDATE_TRIPLE = ERR_BASE_POINT_GRAPHENGINE + -6;  				/*update graph triple failed.*/
	const static int32 ERR_GRAPH_SEARCH_TRIPLE = ERR_BASE_POINT_GRAPHENGINE + -7;  				/*search graph triple failed.*/
	const static int32 ERR_GRAPH_DROP = ERR_BASE_POINT_GRAPHENGINE + -8;  						/*drop graph failed.*/
	const static int32 ERR_GRAPH_PARSE_FILE_NOT_EXIST = ERR_BASE_POINT_GRAPHENGINE + -9;  		/*Parse file does not exist*/
	const static int32 ERR_GRAPH_RESULT_TYPE = ERR_BASE_POINT_GRAPHENGINE + -10;				/*Reuslt type can not be None*/
	const static int32 ERR_GRAPH_UNIVERSAL_POINT = ERR_BASE_POINT_GRAPHENGINE + -11;				

	//ERR_BASE_POINT_QUERYENGINE start
	const static int32 ERR_QUERYENGINE_CONTAINER =  ERR_BASE_POINT_QUERYENGINE + -1;
	const static int32 ERR_QUERYENGINE_INTERRUPTED =  ERR_BASE_POINT_QUERYENGINE + -2;
	const static int32 ERR_QUERYENGINE_TIMEOUT =  ERR_BASE_POINT_QUERYENGINE + -3;


    static const char* GetErrorMessage (const ErrorCode_t error) ;
    static void ThrowExceptionsByCode (const ErrorCode_t error) ;
    static void ThrowExceptionsByCode (const ErrorCode_t error, const char* msg);

    //inits the message and error code.
    virtual void Init (const ErrorCode_t error) ;
    virtual void Init (const ErrorCode_t error, const char *msg) ;
    
public:
    explicit XdbBaseException(const ErrorCode_t errcode);
    XdbBaseException(const ErrorCode_t errcode, const char *msg) ;
    virtual ~XdbBaseException() throw() {}

    // Enable each level of catch block to incrementally append info about 
    // the exception, by appending extra info to the info buffer.
    void append_info(const char *info)
    {
           if (!info) return ; 
           
          //strncat (m_infobuf, "|", strlen ("|"));
          //strncat (m_infobuf, info, strlen(info));      
          uint32 left_chars = EXCEPTION_INFO_BUFLEN - (uint32)strlen (m_infobuf) ;
          if (left_chars > 1 && strlen (info))     
              snprintf (m_infobuf+strlen (m_infobuf), left_chars-1, "|%s", info) ;
    };
    
    void append_info(const std::string& info_string)
    {
        //strncat (m_infobuf, info_string.c_str(), info_string.length());
        uint32 left_chars = EXCEPTION_INFO_BUFLEN - (uint32)strlen (m_infobuf) ;
        if (left_chars > 1)
            snprintf (m_infobuf+strlen (m_infobuf), left_chars-1, "|%s", info_string.c_str());
    }; 

    // Get error code.
    inline ErrorCode_t getErrorCode() const { return m_errorCode; }
    
    const char* what () { return m_infobuf; };  
    // Implement other virtual functions in std::exception.
};

///*************************************************************************************************/
// These following codes describe the second-level base classes. 
// 7 sub-base classes include ServerFrameworkException, DatabaseException, ExecutionEngineException, StorageEngineException, OtherBaseException , 
// QueryEngineException, and FullTextException, which correspond to 'Error_Message_Net', 'Error_Message_DB', 'Error_Message_Array_Execution', 'Error_Message_StorageEngine', 
//  'Error_Message_Other' , 'Error_Message_QueryEngine' and 'Error_Message_Array_FullText' ,''.
// Using the top base exception class directly are not recommended. Using the specific exception class to indicate which type exception occurs.
//
//
//the base class for array of 'Error_Message_Array_Execution'.
class FounderXDB_EXPORT ExecutionEngineException : public virtual XdbBaseException
{
public:
    // Constructor.
    explicit ExecutionEngineException(const ErrorCode_t errcode) ;
    ExecutionEngineException(const ErrorCode_t errcode, const char *msg);
    /// Destructor
    virtual ~ExecutionEngineException() throw() {}
};

// Base exception for all other exceptions in fulltext.
// Virtual inherit form this class.
// the base class for array of 'Error_Message_Array_FullText', all about the fulltext exceptions are derived from this class.
class FounderXDB_EXPORT FullTextException : public virtual XdbBaseException
{
public:
    // Constructor.
    explicit FullTextException(const ErrorCode_t errcode) ;
    FullTextException(const ErrorCode_t errcode, const char *msg);
    /// Destructor
    virtual ~FullTextException() throw() {}
};

// Base exception for all other exceptions in storage engine subsystem.
// Virtual inherit from this class.
//the base class for array of 'Error_Message_StorageEngine', all about the storage engine exceptions are dervied from this class.
class FounderXDB_EXPORT StorageEngineException : public virtual XdbBaseException
{
public:
    // Constructor.
    explicit StorageEngineException(const ErrorCode_t errcode) ;
    StorageEngineException(const ErrorCode_t errcode, const char *msg) ;
    /// Destructor
    virtual ~StorageEngineException() throw() {}
};

// the base class for array of 'Error_Message_Other', all about the other exceptions are derived from this class. 
class FounderXDB_EXPORT OtherBaseException : public virtual XdbBaseException
{
public:
     explicit OtherBaseException (const ErrorCode_t errcode);
     OtherBaseException (const ErrorCode_t errcode, const char* msg);
     virtual ~OtherBaseException () throw() {}
};

//the base class for array of 'Error_Message_DB', all about the db execeptions are derived from this classed.
class FounderXDB_EXPORT DatabaseException : public virtual XdbBaseException
{
public:
     explicit DatabaseException (const ErrorCode_t errcode);
     DatabaseException (const ErrorCode_t errcode, const char* msg ) ;
     virtual ~DatabaseException() throw() {}
} ;
// Base exception for all other exceptions in server framework subsystem.
// Virtual inherit from this class. for the array of 'Error_Message_Net'
class FounderXDB_EXPORT ServerFrameworkException : public virtual XdbBaseException
{
public: 
    explicit ServerFrameworkException (const ErrorCode_t error) ;
    
    ServerFrameworkException (const ErrorCode_t error, const char* msg) ;
    virtual ~ServerFrameworkException () throw() {}
};

//the base class for the array of 'Error_Message_QueryEngine', all about the inquiry exceptions are derived from this class.
class FounderXDB_EXPORT QueryEngineException : public virtual XdbBaseException 
{
public:
      explicit QueryEngineException (const ErrorCode_t error) ;
      QueryEngineException (const ErrorCode_t error, const char* msg );
      virtual ~QueryEngineException() throw() {}
} ;


//the exception for 'ERR_OUT_OF_MEMORY'.
class FounderXDB_EXPORT OutOfMemoryException : public virtual OtherBaseException 
{
    //OutOfMemoryException (const OutOfMemoryException& ) ;
    OutOfMemoryException& operator = (const OutOfMemoryException&) ;

    const static ErrorCode_t error_code  = ERR_OUT_OF_MEMORY ;  
public:
    OutOfMemoryException();
    explicit OutOfMemoryException(size_t bytes);
    OutOfMemoryException(size_t bytes, const char* msg);
    virtual ~OutOfMemoryException() throw() {}
};

//the exception for 'ERR_OBJECT_NOT_DESTROYED'.
class FounderXDB_EXPORT ObjectNotDestroyedException : public virtual OtherBaseException 
{
    //ObjectNotDestroyedException (const ObjectNotDestroyedException&);
    ObjectNotDestroyedException & operator = (const ObjectNotDestroyedException&);
    const static ErrorCode_t error_code  = ERR_OBJECT_NOT_DESTROYED ;  
public:
    explicit ObjectNotDestroyedException(size_t num_leaked) ;
    ObjectNotDestroyedException(size_t num_leaked, const char* msg) ;
    virtual ~ObjectNotDestroyedException() throw() {}
};

//the exception for 'ERR_NOT_SUPPORTED_FEATURE'.
class FounderXDB_EXPORT NotSupportedException : public virtual OtherBaseException
{
    //NotSupportedException (const NotSupportedException&);
    NotSupportedException & operator = (const NotSupportedException&);
    const static ErrorCode_t error_code  = ERR_NOT_SUPPORTED_FEATURE ;  
public:
    NotSupportedException();
    explicit NotSupportedException(const char* msg);
    virtual ~NotSupportedException() throw() {}
};

//the exception for 'ERR_VIOLATE_OUT_OF_BOUNDARY'.
class FounderXDB_EXPORT OutOfBoundaryException : public virtual OtherBaseException
{
    ///disable the copy constructor and assignment operator
    //OutOfBoundaryException (const OutOfBoundaryException&) ; 
    OutOfBoundaryException& operator = (const OutOfBoundaryException&) ; 

    const static ErrorCode_t error_code  = ERR_VIOLATE_OUT_OF_BOUNDARY ;      
public:
    OutOfBoundaryException ();
    explicit OutOfBoundaryException (const char* msg);
    virtual ~OutOfBoundaryException () throw() {}
    
} ;

//the exception for 'ERR_NULL_POINTER'.
class FounderXDB_EXPORT NullPointerException : public virtual OtherBaseException
{
    ///disable the copy constructor and assignment operator
    //NullPointerException (const NullPointerException&) ; 
    NullPointerException& operator = (const NullPointerException&) ; 

    const static ErrorCode_t error_code  = ERR_NULL_POINTER;      
public:
    NullPointerException ();
    explicit NullPointerException (const char* msg);
    virtual ~NullPointerException () throw() {}
} ;

//the exception for 'ERR_NULL_POINTER'.
class FounderXDB_EXPORT EmptyStringException : public virtual OtherBaseException
{
    ///disable the copy constructor and assignment operator
    //EmptyStringException (const EmptyStringException&) ; 
    EmptyStringException& operator = (const EmptyStringException&) ; 

    const static ErrorCode_t error_code  = ERR_EMPTY_STRING;      
public:
    EmptyStringException ();
    explicit EmptyStringException (const char* msg);
    virtual ~EmptyStringException () throw() {}
} ;

//the exception for 'ERR_PARAMETER_NUMBER'.
class FounderXDB_EXPORT CommandSyntaxException : public virtual OtherBaseException
{
    ///disable the copy constructor and assignment operator
    //ParameterNumberException (const ParameterNumberException&) ; 
    CommandSyntaxException& operator = (const CommandSyntaxException&) ; 

    const static ErrorCode_t error_code  = ERR_COMMAND_SYNTAX;      
public:
    CommandSyntaxException ();
    explicit CommandSyntaxException (const char* msg);
    virtual ~CommandSyntaxException () throw() {}
} ;
    
class FounderXDB_EXPORT UnknownException : public virtual OtherBaseException
{
    ///disable the copy constructor and assignment operator
    //ParameterNumberException (const ParameterNumberException&) ; 
    UnknownException& operator = (const UnknownException&) ; 

    const static ErrorCode_t error_code  = ERR_UNKNOWN_EXCEPTION;      
public:
    UnknownException ();
    explicit UnknownException (const char* msg);
    virtual ~UnknownException () throw() {}
} ;

//throws this exception, ViolateSegmentationException, when segmentation violation error occurs.
class FounderXDB_EXPORT ViolateSegmentationException : public virtual OtherBaseException
{
    //ViolateSegmentationException (const ViolateSegmentationException& ) ; 
    ViolateSegmentationException& operator = (const ViolateSegmentationException&) ;
    const static ErrorCode_t error_code  = ERR_VIOLATE_SEGMENTATION_FAULT ;
public : 
    ViolateSegmentationException ();
    explicit ViolateSegmentationException (const char* msg);
    virtual ~ViolateSegmentationException () throw() {}
} ;

//throws this exception when the user is an invalid user in our system.
class FounderXDB_EXPORT InvalidUserException : public virtual DatabaseException 
{
    ///disable the copy construtor and assignment operator
    //InvalidUserException (const InvalidUserException&) ; 
    InvalidUserException& operator = (const InvalidUserException&) ; 

    const static ErrorCode_t error_code  = ERR_DB_INVALIDE_USER ;      
public:
    InvalidUserException ();    
    explicit InvalidUserException (const char* msg);    
    virtual ~InvalidUserException () throw() {}
}; 

//the exception for 'ERR_DB_INVALIDE_PASSWORD'.
class FounderXDB_EXPORT InvalidPasswordException : public virtual DatabaseException
{     
    ///disable the copy constructor and assignment operator
    //InvalidPasswordException (const InvalidPasswordException&) ; 
    InvalidPasswordException& operator = (const InvalidPasswordException&) ; 

    const static ErrorCode_t error_code  = ERR_DB_INVALIDE_PASSWORD ;      
public:
    InvalidPasswordException (); 
    explicit InvalidPasswordException (const char* msg); 
    virtual ~InvalidPasswordException () throw() {}
} ;

//the exception for 'ERR_DB_INVALIDE_STATEMENT'.
class FounderXDB_EXPORT InvalidQueryStatementException : public virtual DatabaseException
{ 
    ///disable the copy constructor and assignment operator
    //InvalidQueryStatement (const InvalidQueryStatement&) ; 
    InvalidQueryStatementException& operator = (const InvalidQueryStatementException&) ; 

    const static ErrorCode_t error_code  = ERR_DB_INVALIDE_STATEMENT ;      
public:
    InvalidQueryStatementException () ;
    explicit InvalidQueryStatementException (const char* msg) ;
    virtual ~InvalidQueryStatementException () throw() {}
    
} ;

//the exception for 'ERR_NET_INVALIDE_IP'.
class FounderXDB_EXPORT InvalidIPAddrException : public virtual ServerFrameworkException
{
    ///disable the copy constructor and assignment operator
    //InvalidIPAddrException (const InvalidIPAddrException&) ; 
    InvalidIPAddrException& operator = (const InvalidIPAddrException&) ; 

    const static ErrorCode_t error_code  = ERR_NET_INVALIDE_IP ;      
public:
    InvalidIPAddrException ();
    explicit InvalidIPAddrException (const char* msg);
    ///same meaning.
    //virtual const char* what () const {return m_infobuf ;}
} ;

class FounderXDB_EXPORT SslException : public virtual ServerFrameworkException
{
    ///disable the copy constructor and assignment operator 
    SslException& operator = (const SslException&) ; 
    const static ErrorCode_t error_code  = ERR_NET_SSL ;      
public:
    SslException ();
    explicit SslException (const char* msg);
} ;

//the exception for 'ERR_NET_SERVER_NOT_OPEN'.
class FounderXDB_EXPORT ServerNotOpenException : public virtual ServerFrameworkException
{
    ///disable the copy constructor and assignment operator
    //ServerNotOpenException (const ServerNotOpenException&) ; 
    ServerNotOpenException& operator = (const ServerNotOpenException&) ; 

    const static ErrorCode_t error_code  = ERR_NET_SERVER_NOT_OPEN ;      
public:
    ServerNotOpenException ();
    explicit ServerNotOpenException (const char* msg);
    virtual ~ServerNotOpenException () throw() {}
} ;

//the exception for 'ERR_NET_TIMEOUT'.
class FounderXDB_EXPORT NetTimeOutException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //NetTimeOutException (const NetTimeOutException&) ; 
    NetTimeOutException& operator = (const NetTimeOutException&) ; 

    const static ErrorCode_t error_code  = ERR_NET_TIMEOUT ;      
public:
    NetTimeOutException ();
    explicit NetTimeOutException (const char* msg);
    virtual ~NetTimeOutException () throw() {}
    
} ;

//the exception for 'ERR_NET_INVALID_CERTIFICATES'.
class FounderXDB_EXPORT InvalidCertificateException : public virtual ServerFrameworkException 
{
    //InvalidCertificateException (const InvalidCertificateException& ) ; 
    InvalidCertificateException& operator = (const InvalidCertificateException&) ;
    const static ErrorCode_t error_code  = ERR_NET_INVALID_CERTIFICATES ;
public :
    InvalidCertificateException ();
    explicit InvalidCertificateException (const char* msg);
    virtual ~InvalidCertificateException () throw() {}
} ;

//the exception for 'ERR_NET_UNKNOWNCMMD'.
class FounderXDB_EXPORT UnkownCommandException : public virtual ServerFrameworkException 
{
    //UnkownCommandException (const UnkownCommandException&) ;
    UnkownCommandException& operator = (const UnkownCommandException&) ;
    const static ErrorCode_t error_code  = ERR_NET_UNKNOWNCMMD ;
public:
    UnkownCommandException () ;
    explicit UnkownCommandException (const char* msg) ;
    virtual ~UnkownCommandException () throw() {}
} ; 

//the exception for 'ERR_NET_CONNECTION_CLOSED'.
class FounderXDB_EXPORT ConnectionClosedException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //ConnectionClosedException (const ConnectionClosedException&) ; 
    ConnectionClosedException& operator = (const ConnectionClosedException&) ; 

    const static ErrorCode_t error_code = ERR_NET_CONNECTION_CLOSED ;      
public:
    ConnectionClosedException ();
    explicit ConnectionClosedException (const char* msg);
    virtual ~ConnectionClosedException () throw() {}

} ;

//the exception for 'ERR_NET_NO_TRANSACTION'.
class FounderXDB_EXPORT NoTransactionException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //NoTransactionException (const NoTransactionException&) ; 
    NoTransactionException& operator = (const NoTransactionException&) ; 

    const static ErrorCode_t error_code = ERR_NET_NO_TRANSACTION ;      
public:
    NoTransactionException ();
    explicit NoTransactionException (const char* msg);
    virtual ~NoTransactionException () throw() {}

} ;

//the exception for 'ERR_NET_TRANSACTION_EXIST'.
class FounderXDB_EXPORT TransactionExistException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //TransactionExistException (const TransactionExistException&) ; 
    TransactionExistException& operator = (const TransactionExistException&) ; 

    const static ErrorCode_t error_code = ERR_NET_TRANSACTION_EXIST ;      
public:
    TransactionExistException ();
    explicit TransactionExistException (const char* msg);
    virtual ~TransactionExistException () throw() {}

} ;

//the exception for 'ERR_NET_DATANODE_DOWN'.
class FounderXDB_EXPORT DataNodeDownException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    DataNodeDownException& operator = (const DataNodeDownException&) ; 

    const static ErrorCode_t error_code = ERR_NET_DATANODE_DOWN ;      
public:
    DataNodeDownException ();
    explicit DataNodeDownException (const char* msg);
    virtual ~DataNodeDownException () throw() {}

} ;

//the exception for 'ERR_NET_CLUSTER_NOT_READY'.
class FounderXDB_EXPORT ClusterEnvironmentException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    ClusterEnvironmentException& operator = (const ClusterEnvironmentException&) ; 

    const static ErrorCode_t error_code = ERR_NET_CLUSTER_NOT_READY ;      
public:
    ClusterEnvironmentException ();
    explicit ClusterEnvironmentException (const char* msg);
    virtual ~ClusterEnvironmentException () throw() {}

} ;

//the exception for 'ERR_NET_EXECUTOR_REQUEST_CREATE_FAILED'.
class FounderXDB_EXPORT ExecutorCreateException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //ExecutorCreateException (const ExecutorCreateException&) ; 
    ExecutorCreateException& operator = (const NoTransactionException&) ; 

    const static ErrorCode_t error_code = ERR_NET_EXECUTOR_REQUEST_CREATE_FAILED ;      
public:
    ExecutorCreateException ();
    explicit ExecutorCreateException (const char* msg);
    virtual ~ExecutorCreateException () throw() {}

} ;

//the exception for 'ERR_NET_NOT_LOGIN'.
class FounderXDB_EXPORT NotLoginExeception : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //NotLoginExeception (const NotLoginExeception&) ; 
    NotLoginExeception& operator = (const NoTransactionException&) ; 

    const static ErrorCode_t error_code = ERR_NET_NOT_LOGIN ;      
public:
    NotLoginExeception ();
    explicit NotLoginExeception (const char* msg);
    virtual ~NotLoginExeception () throw() {}
} ;

class FounderXDB_EXPORT NotRecursiveLogin : public virtual ServerFrameworkException
{
    ///disable the copy constructor and assignment operator
    //NotLoginExeception (const NotLoginExeception&) ; 
    NotRecursiveLogin& operator = (const NotRecursiveLogin&) ; 

    const static ErrorCode_t error_code = ERR_NET_RECURSIVE_LOGIN ;      
public:
    NotRecursiveLogin ();
    explicit NotRecursiveLogin (const char* msg);
    virtual ~NotRecursiveLogin () throw() {}
};
//the exception  for 'ERR_NET_NOT_LOGIN'.
class FounderXDB_EXPORT SocketException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //SocketException (const SocketException&) ; 
    SocketException& operator = (const SocketException&) ; 

    const static ErrorCode_t error_code = ERR_SOCKET_EXCEPTION;      
public:
    SocketException ();
    explicit SocketException (const char* msg);
    SocketException (const int val, const char* msg);
    virtual ~SocketException () throw() {}

} ;
class FounderXDB_EXPORT SrvcfgFileException : public virtual ServerFrameworkException
{    
    ///disable the copy constructor and assignment operator
    //SocketException (const SocketException&) ; 
    SrvcfgFileException& operator = (const SrvcfgFileException&) ; 

    const static ErrorCode_t error_code = ERR_NET_SVRCFG_FILE; 
public:
    SrvcfgFileException ();
    explicit SrvcfgFileException (const char* msg);
    SrvcfgFileException (const int val, const char* msg);
    virtual ~SrvcfgFileException () throw() {}

} ;

//throws this exception, OpenContainerException, when do openning container operation.
class FounderXDB_EXPORT OpenContainerException : public virtual ExecutionEngineException
{
    //OpenContainerException (const OpenContainerException&) ;
    OpenContainerException& operator = (const OpenContainerException&) ; 
    const static ErrorCode_t error_code  = ERR_OPEN_CONTAINER ;
public :
    OpenContainerException () ;
    explicit OpenContainerException (const char* msg) ;
    virtual ~OpenContainerException () throw() {}
}; 

//throws ContainerNotExistException when the conatainer does not exist.
class FounderXDB_EXPORT ContainerNotExistException : public virtual ExecutionEngineException 
{
    //ContainerNotExistException (const ContainerNotExistException& ) ;
    ContainerNotExistException& operator = (const ContainerNotExistException&);
    const static ErrorCode_t error_code  = ERR_NOTEXISTS_CONTAINER ;
public:
    ContainerNotExistException () ;
    explicit ContainerNotExistException (const char* msg) ;
    virtual ~ContainerNotExistException () throw() {}

} ;

class FounderXDB_EXPORT ReIndexException : public virtual ExecutionEngineException 
{
    //ContainerNotExistException (const ContainerNotExistException& ) ;
    ReIndexException& operator = (const ReIndexException&);
    const static ErrorCode_t error_code  = ERR_RE_INDEX ;
public:
    ReIndexException () ;
    explicit ReIndexException (const char* msg) ;
    virtual ~ReIndexException () throw() {}

} ;

//throws GrantNodeErroException when the conatainer does not exist.
class FounderXDB_EXPORT GrantNodeErroException : public virtual ExecutionEngineException 
{
    //GrantNodeErroException (const GrantNodeErroException& ) ;
    GrantNodeErroException& operator = (const GrantNodeErroException&);
    const static ErrorCode_t error_code  = ERR_GRANT_NODE ;
public:
    GrantNodeErroException () ;
    explicit GrantNodeErroException (const char* msg) ;
    virtual ~GrantNodeErroException () throw() {}

} ;
//throws CheckNodePrivErroException when the conatainer does not exist.
class FounderXDB_EXPORT CheckNodePrivErroException : public virtual ExecutionEngineException 
{
    //CheckNodePrivErroException (const CheckNodePrivErroException& ) ;
    CheckNodePrivErroException& operator = (const CheckNodePrivErroException&);
    const static ErrorCode_t error_code  = ERR_CHECK_NODE_PRIV ;
public:
    CheckNodePrivErroException () ;
    explicit CheckNodePrivErroException (const char* msg) ;
    virtual ~CheckNodePrivErroException () throw() {}

} ;
//throws RevokeNodeErroException when the conatainer does not exist.
class FounderXDB_EXPORT RevokeNodeErroException : public virtual ExecutionEngineException 
{
    //RevokeNodeErroException (const RevokeNodeErroException& ) ;
    RevokeNodeErroException& operator = (const RevokeNodeErroException&);
    const static ErrorCode_t error_code  = ERR_REVOKE_NODE ;
public:
    RevokeNodeErroException () ;
    explicit RevokeNodeErroException (const char* msg) ;
    virtual ~RevokeNodeErroException () throw() {}

} ;

//throws ListNodePrivErroException when the conatainer does not exist.
class FounderXDB_EXPORT ListNodePrivErroException : public virtual ExecutionEngineException 
{
    //ListNodePrivErroException (const ListNodePrivErroException& ) ;
    ListNodePrivErroException& operator = (const ListNodePrivErroException&);
    const static ErrorCode_t error_code  = ERR_LIST_PRIV_NODE ;
public:
    ListNodePrivErroException () ;
    explicit ListNodePrivErroException (const char* msg) ;
    virtual ~ListNodePrivErroException () throw() {}

} ;

//throws ContainerNotCommitException when the container does not commit.
class FounderXDB_EXPORT ContainerNotCommitException : public virtual ExecutionEngineException 
{
    //ContainerNotCommitException (const ContainerNotCommitException& ) ;
    ContainerNotCommitException& operator = (const ContainerNotCommitException&);
    const static ErrorCode_t error_code  = ERR_CONTAINER_NOT_COMMIT ;
public:
    ContainerNotCommitException () ;
    explicit ContainerNotCommitException (const char* msg) ;
    virtual ~ContainerNotCommitException () throw() {}

} ;

//throws IDOverFlowException when ID over flow.
class FounderXDB_EXPORT IDOverFlowException : public virtual ExecutionEngineException 
{
    //IDOverFlowException (const IDOverFlowException& ) ;
    IDOverFlowException& operator = (const IDOverFlowException&);
    const static ErrorCode_t error_code  = ERR_ID_OVERFLOW ;
public:
    IDOverFlowException () ;
    explicit IDOverFlowException (const char* msg) ;
    virtual ~IDOverFlowException () throw() {}

} ;



//throws LargeObjectNotExistException when the conatainer does not exist.
class FounderXDB_EXPORT LargeObjectNotExistException : public virtual ExecutionEngineException 
{
    //LargeObjectNotExistException (const LargeObjectNotExistException& ) ;
    LargeObjectNotExistException& operator = (const LargeObjectNotExistException&);
    const static ErrorCode_t error_code  = ERR_NOTEXISTS_LARGE_OBJECT ;
public:
    LargeObjectNotExistException () ;
    explicit LargeObjectNotExistException (const char* msg) ;
    virtual ~LargeObjectNotExistException () throw() {}

} ;

//throws LargeObjectException when the conatainer does not exist.
class FounderXDB_EXPORT LargeObjectException : public virtual ExecutionEngineException 
{
    //LargeObjectException (const LargeObjectException& ) ;
    LargeObjectException& operator = (const LargeObjectException&);
    const static ErrorCode_t error_code  = ERR_LARGE_OBJECT ;
public:
    LargeObjectException () ;
    explicit LargeObjectException (const char* msg) ;
    virtual ~LargeObjectException () throw() {}

} ;

//throws AddDocumentException when adding a doucment.
class FounderXDB_EXPORT AddDocumentException : public virtual ExecutionEngineException
{
    //AddDocumentException (const AddDocumentException& ) ;
    AddDocumentException& operator = (const AddDocumentException&);
    const static ErrorCode_t error_code  = ERR_ADD_DOCUMENT ;
public:
    AddDocumentException () ;
    explicit AddDocumentException (const char* msg) ;
    virtual ~AddDocumentException () throw() {}
};

//throws TableSpaceNotExistException when a table does not exit.
class FounderXDB_EXPORT TableSpaceNotExistException : public virtual ExecutionEngineException
{
    //TableSpaceException(const TableSpaceException& ) ;
    TableSpaceNotExistException& operator = (const TableSpaceNotExistException&);
    const static ErrorCode_t error_code = ERR_TABLESPACE_NOT_EXIST ;
public :
    TableSpaceNotExistException () ;
    explicit TableSpaceNotExistException (const char* msg) ;
    virtual ~TableSpaceNotExistException () throw() {}
};

//throws CreateContainerException when creating a container.
class FounderXDB_EXPORT CreateContainerException : public virtual ExecutionEngineException
{
    //CreateContainerException (const CreateContainerException& ) ;
    CreateContainerException& operator = (const CreateContainerException&) ;
    const static ErrorCode_t error_code  = ERR_CREATE_CONTAINER ;
public :
    CreateContainerException () ;
    explicit CreateContainerException (const char* msg) ;
    virtual ~CreateContainerException () throw() {}
};

class FounderXDB_EXPORT DumpContainerException : public virtual ExecutionEngineException
{
    //DumpContainerException (const DumpContainerException& ) ;
    DumpContainerException& operator = (const DumpContainerException&) ;
    const static ErrorCode_t error_code  = ERR_DUMP_CONTAINER ;
public :
    DumpContainerException () ;
    explicit DumpContainerException (const char* msg) ;
    virtual ~DumpContainerException () throw() {}
};

class FounderXDB_EXPORT LoadContainerException : public virtual ExecutionEngineException
{
    //LoadContainerException (const LoadContainerException& ) ;
    LoadContainerException& operator = (const LoadContainerException&) ;
    const static ErrorCode_t error_code  = ERR_LOAD_CONTAINER ;
public :
    LoadContainerException () ;
    explicit LoadContainerException (const char* msg) ;
    virtual ~LoadContainerException () throw() {}
};

//throws QueryStatementException if the the query statements are invalid.
class FounderXDB_EXPORT QueryStatementException : public virtual ExecutionEngineException
{
    //QueryStatementException (const QueryStatementException& ) ;
    QueryStatementException& operator = (const QueryStatementException&) ;
    const static ErrorCode_t error_code  = ERR_QUERY ;
public:
    QueryStatementException ();
    explicit QueryStatementException (const char* msg);
    virtual ~QueryStatementException () throw() {}
} ;

//throws RemoveContainerException when we do container-remove operation.  
class FounderXDB_EXPORT RemoveContainerException : public virtual ExecutionEngineException
{
    //RemoveContainerException (const RemoveContainerException& ) ;
    RemoveContainerException& operator =(const RemoveContainerException&) ;
    const static ErrorCode_t error_code  = ERR_REMOVE_CONTAINER ; 
public:
    RemoveContainerException ();
    explicit RemoveContainerException (const char* msg);
    virtual ~RemoveContainerException () throw() {}
} ;
//the exception for 'ERR_CONTAINER_ALREADY_EXISTS'.
class FounderXDB_EXPORT ContainerAlreadyExistException : public virtual ExecutionEngineException
{
    //ContainerAlreadyExistException (const ContainerAlreadyExistException&);   
    ContainerAlreadyExistException& operator = (const ContainerAlreadyExistException&) ;
    const static  ErrorCode_t error_code  = ERR_CONTAINER_ALREADY_EXISTS;   
public:
    ContainerAlreadyExistException () ;
    explicit ContainerAlreadyExistException (const char* msg) ;
    virtual ~ContainerAlreadyExistException () throw() {}
} ;

//the exception for 'ERR_CONTAINER_ALREADY_EXISTS'.
class FounderXDB_EXPORT ContainerFullException : public virtual ExecutionEngineException
{
    //ContainerFullException (const ContainerFullException&);   
    ContainerFullException& operator = (const ContainerFullException&) ;
    const static  ErrorCode_t error_code  = ERR_CONTAINER_FULL;   
public:
    ContainerFullException () ;
    explicit ContainerFullException (const char* msg) ;
    virtual ~ContainerFullException () throw() {}
} ;

//the exception for 'ERR_UPDATE_DOCUMENT'.
class FounderXDB_EXPORT UpdateDocumentsException : public virtual ExecutionEngineException
{
    //UpdateDocumentsException (const UpdateDocumentsException&);
    UpdateDocumentsException& operator = (const UpdateDocumentsException&) ;
    const static  ErrorCode_t error_code  = ERR_UPDATE_DOCUMENT ;
public:
    UpdateDocumentsException ();
    explicit UpdateDocumentsException (const char* msg);
    virtual ~UpdateDocumentsException () throw() {} 
} ;

//the exception for 'ERR_REMOVE_DOCUMENT'.
class FounderXDB_EXPORT RemoveDocumentException : public virtual ExecutionEngineException 
{
    // RemoveDocumentException (const RemoveDocumentException& ) ;
    RemoveDocumentException& operator = (const RemoveDocumentException& );
    const static  ErrorCode_t error_code  = ERR_REMOVE_DOCUMENT ;
public:
    RemoveDocumentException () ;
    explicit RemoveDocumentException (const char* msg) ;
    virtual  ~RemoveDocumentException () throw() {}
} ;

//the exception for 'ERR_REMOVE_DOCUMENT'.
class FounderXDB_EXPORT RenameContainerException : public virtual ExecutionEngineException 
{
    // RemoveDocumentException (const RemoveDocumentException& ) ;
    RenameContainerException& operator = (const RenameContainerException& );
    const static  ErrorCode_t error_code  = ERR_RENAME_CONTAINER ;
public:
    RenameContainerException () ;
    explicit RenameContainerException (const char* msg) ;
    virtual  ~RenameContainerException () throw() {}
} ;

//the exception for 'ERR_CANNOT_OPEN_CONTAINER'.
class FounderXDB_EXPORT CanNotOpenContainerException : public virtual ExecutionEngineException
{
    //CanNotOpenContainerException (const CanNotOpenContainerException &);
    CanNotOpenContainerException & operator = (const CanNotOpenContainerException&);
    const static ErrorCode_t error_code  = ERR_CANNOT_OPEN_CONTAINER ;
public:
    CanNotOpenContainerException ();
    explicit CanNotOpenContainerException (const char* msg);
    virtual  ~CanNotOpenContainerException() throw() {}
} ;

//the exception for 'ERR_SQL_DATABASE'.
class FounderXDB_EXPORT SqlDatabaseException : public virtual ExecutionEngineException
{
    //SqlDatabaseException (const SqlDatabaseException &);
    SqlDatabaseException & operator = (const SqlDatabaseException&);
    const static ErrorCode_t error_code  = ERR_SQL_DATABASE ;
public:
    SqlDatabaseException ();
    explicit SqlDatabaseException (const char* msg);
    virtual  ~SqlDatabaseException() throw() {}
} ;

class FounderXDB_EXPORT DeleteDocitemCascadeException : public virtual ExecutionEngineException
{
    //DeleteDocitemCascadeException (const DeleteDocitemCascadeException &);
    DeleteDocitemCascadeException & operator = (const DeleteDocitemCascadeException&);
    const static ErrorCode_t error_code  = ERROR_DELETE_DOCITEM_CASCADE ;
public:
    DeleteDocitemCascadeException ();
    explicit DeleteDocitemCascadeException (const char* msg);
    virtual  ~DeleteDocitemCascadeException() throw() {}
} ;
//the exception for 'ERR_ADD_INDEX'.
class FounderXDB_EXPORT AddIndexException : public virtual ExecutionEngineException
{
    //AddIndexException (const AddIndexException &);
    AddIndexException & operator = (const AddIndexException&);
    const static ErrorCode_t error_code  =  ERR_ADD_INDEX;
public:
    AddIndexException ();
    explicit AddIndexException (const char* msg);
    virtual  ~AddIndexException() throw() {}
} ;

//the exception for 'ERR_REMOVE_INDEX'.
class FounderXDB_EXPORT RemoveIndexException : public virtual ExecutionEngineException
{
    //RemoveIndexException (const RemoveIndexException &);
    RemoveIndexException & operator = (const RemoveIndexException&);
    const static ErrorCode_t error_code  =  ERR_REMOVE_INDEX;
public:
    RemoveIndexException ();
    explicit RemoveIndexException (const char* msg);
    virtual  ~RemoveIndexException() throw() {}
} ;

//the exception for 'ERR_NO_PATHINDEX'.
class FounderXDB_EXPORT NoPathIndexException : public virtual ExecutionEngineException
{
    //NoPathIndexException (const NoPathIndexException &);
    NoPathIndexException & operator = (const NoPathIndexException&);
    const static ErrorCode_t error_code  =  ERR_NO_PATHINDEX;
public:
    NoPathIndexException ();
    explicit NoPathIndexException (const char* msg);
    virtual  ~NoPathIndexException() throw() {}
} ;

//the exception for 'ERR_HAVE_PATHINDEX'.
class FounderXDB_EXPORT PathIndexFailException : public virtual ExecutionEngineException
{
    //PathIndexFailException (const PathIndexFailException &);
    PathIndexFailException & operator = (const PathIndexFailException&);
    const static ErrorCode_t error_code  =  ERR_PATHINDEX_FAIL;
public:
    PathIndexFailException ();
    explicit PathIndexFailException (const char* msg);
    virtual  ~PathIndexFailException() throw() {}
} ;

//the exception for 'ERR_PATHINDEX_REPEAT'.
class FounderXDB_EXPORT PathIndexRepeatException : public virtual ExecutionEngineException
{
    //PathIndexRepeatException (const PathIndexRepeatException &);
    PathIndexRepeatException & operator = (const PathIndexRepeatException&);
    const static ErrorCode_t error_code  =  ERR_PATHINDEX_REPEAT;
public:
    PathIndexRepeatException ();
    explicit PathIndexRepeatException (const char* msg);
    virtual  ~PathIndexRepeatException() throw() {}
} ;

//the exception for 'ERR_PATHINDEX_FAIL'.
class FounderXDB_EXPORT HavePathIndexException : public virtual ExecutionEngineException
{
    //PathIndexFailException (const PathIndexFailException &);
    HavePathIndexException & operator = (const HavePathIndexException&);
    const static ErrorCode_t error_code  =  ERR_HAVE_PATHINDEX;
public:
    HavePathIndexException ();
    explicit HavePathIndexException (const char* msg);
    virtual  ~HavePathIndexException() throw() {}
} ;


//the exception for 'ERR_PS_EXECUTE'.
class FounderXDB_EXPORT PSExecuteException : public virtual ExecutionEngineException
{
    //PSExecuteException (const PSExecuteException &);
    PSExecuteException & operator = (const PSExecuteException&);
    const static ErrorCode_t error_code  =  ERR_PS_EXECUTE;
public:
    PSExecuteException ();
    explicit PSExecuteException (const char* msg);
    virtual  ~PSExecuteException() throw() {}
} ;

//the exception for 'ERR_PARAMETER_INVALID'.
class FounderXDB_EXPORT ParameterInvalidException : public virtual ExecutionEngineException
{
    //ParameterInvalidException (const ParameterInvalidException &);
    ParameterInvalidException & operator = (const ParameterInvalidException&);
    const static ErrorCode_t error_code  =  ERR_PARAMETER_INVALID ;
public:
    ParameterInvalidException ();
    explicit ParameterInvalidException (const char* msg);
    virtual  ~ParameterInvalidException() throw() {}
} ;

//the exception for 'ERR_CAN_NOT_OPEN_SYSTEM_DB'.
class FounderXDB_EXPORT CanNotOpenSystemDBException : public virtual ExecutionEngineException
{
    //CanNotOpenSystemDBException (const CanNotOpenSystemDBException &);
    CanNotOpenSystemDBException & operator = (const CanNotOpenSystemDBException&);
    const static ErrorCode_t error_code  =  ERR_CAN_NOT_OPEN_SYSTEM_DB ;
public:
    CanNotOpenSystemDBException ();
    explicit CanNotOpenSystemDBException (const char* msg);
    virtual  ~CanNotOpenSystemDBException() throw() {}
} ;

//the exception for 'ERR_CAN_NOT_ACCESS_SYSTEM_DB'.
class FounderXDB_EXPORT CanNotAccessSystemDBException : public virtual ExecutionEngineException
{
    //CanNotAccessSystemDBException (const CanNotAccessSystemDBException &);
    CanNotAccessSystemDBException & operator = (const CanNotAccessSystemDBException&);
    const static ErrorCode_t error_code  =  ERR_CAN_NOT_ACCESS_SYSTEM_DB;
public:
    CanNotAccessSystemDBException ();
    explicit CanNotAccessSystemDBException (const char* msg);
    virtual  ~CanNotAccessSystemDBException() throw() {}
} ;

class FounderXDB_EXPORT CanNotAccessCollectionException : public virtual ExecutionEngineException
{
    //CanNotAccessCollectionException (const CanNotAccessCollectionException &);
    CanNotAccessCollectionException & operator = (const CanNotAccessCollectionException&);
    const static ErrorCode_t error_code  =  ERR_CAN_NOT_ACCESS_COLLECTION;
public:
    CanNotAccessCollectionException ();
    explicit CanNotAccessCollectionException (const char* msg);
    virtual  ~CanNotAccessCollectionException() throw() {}
} ;


class FounderXDB_EXPORT CanNotAccessDocFileException : public virtual ExecutionEngineException
{
    //CanNotAccessDocFileException (const CanNotAccessDocFileException &);
    CanNotAccessDocFileException & operator = (const CanNotAccessDocFileException&);
    const static ErrorCode_t error_code  =  ERR_CAN_NOT_ACCESS_DOCFILE;
public:
    CanNotAccessDocFileException ();
    explicit CanNotAccessDocFileException (const char* msg);
    virtual  ~CanNotAccessDocFileException() throw() {}
} ;

class FounderXDB_EXPORT DatabaseNamedSyntaxException: public virtual ExecutionEngineException
{
    //DatabaseNamedSyntaxException (const DatabaseNamedSyntaxException &);
    DatabaseNamedSyntaxException & operator = (const DatabaseNamedSyntaxException&);
    const static ErrorCode_t error_code  =  ERR_DATABASE_NAMED_SYNTAX;
public:
    DatabaseNamedSyntaxException ();
    explicit DatabaseNamedSyntaxException (const char* msg);
    virtual  ~DatabaseNamedSyntaxException() throw() {}
} ;

class FounderXDB_EXPORT DatabaseUsedException: public virtual ExecutionEngineException
{
    //DatabaseUsedException (const DatabaseUsedException &);
    DatabaseUsedException & operator = (const DatabaseUsedException&);
    const static ErrorCode_t error_code  =  ERR_DATABASE_USED;
public:
    DatabaseUsedException ();
    explicit DatabaseUsedException (const char* msg);
    virtual  ~DatabaseUsedException() throw() {}
} ;

class FounderXDB_EXPORT DatabaseAlreadyExistException : public virtual ExecutionEngineException
{
    //DatabaseAlreadyExistException (const DatabaseAlreadyExistException &);
    DatabaseAlreadyExistException & operator = (const DatabaseAlreadyExistException&);
    const static ErrorCode_t error_code  =  ERR_DATABASE_ALREADY_EXIST;
public:
    DatabaseAlreadyExistException ();
    explicit DatabaseAlreadyExistException (const char* msg);
    virtual  ~DatabaseAlreadyExistException() throw() {}
} ;

//the exception for 'ERR_DATABASE_NOT_EXIST'.
class FounderXDB_EXPORT DatabaseNotExistException : public virtual ExecutionEngineException
{
    //DatabaseNotExistException (const DatabaseNotExistException &);
   DatabaseNotExistException & operator = (const DatabaseNotExistException&);
    const static ErrorCode_t error_code  =  ERR_DATABASE_NOT_EXIST;
public:
    DatabaseNotExistException ();
    explicit DatabaseNotExistException (const char* msg);
    virtual  ~DatabaseNotExistException() throw() {}
} ;

class FounderXDB_EXPORT DatabaseDeleteFailException : public virtual ExecutionEngineException
{
    DatabaseDeleteFailException & operator = (const DatabaseDeleteFailException&);
    const static ErrorCode_t error_code  =  ERR_DATABASE_DELETE_FAIL;
public:
    DatabaseDeleteFailException ();
    explicit DatabaseDeleteFailException (const char* msg);
    virtual  ~DatabaseDeleteFailException() throw() {}
} ;
class FounderXDB_EXPORT CreateTableSpaceException : public virtual ExecutionEngineException
{
    //DatabaseNotExistException (const DatabaseNotExistException &);
    CreateTableSpaceException & operator = (const CreateTableSpaceException&);
    const static ErrorCode_t error_code  =  ERR_CREATE_TABLE_SPACE;
public:
    CreateTableSpaceException ();
    explicit CreateTableSpaceException (const char* msg);
    virtual  ~CreateTableSpaceException() throw() {}
} ;

class FounderXDB_EXPORT TableSpaceAlreadyExistException : public virtual ExecutionEngineException
{
    //TableSpaceAlreadyExistException (const TableSpaceAlreadyExistException &);
    TableSpaceAlreadyExistException & operator = (const TableSpaceAlreadyExistException&);
    const static ErrorCode_t error_code  =  ERR_TABLE_SPACE_ALREADYEXIST;
public:
    TableSpaceAlreadyExistException ();
    explicit TableSpaceAlreadyExistException (const char* msg);
    virtual  ~TableSpaceAlreadyExistException() throw() {}
} ;

class FounderXDB_EXPORT SlaveReadOnlyException : public virtual ExecutionEngineException
{
    //TableSpaceAlreadyExistException (const TableSpaceAlreadyExistException &);
    SlaveReadOnlyException & operator = (const SlaveReadOnlyException&);
    const static ErrorCode_t error_code  =  ERR_SLAVE_READ_ONLY;
public:
    SlaveReadOnlyException ();
    explicit SlaveReadOnlyException (const char* msg);
    virtual  ~SlaveReadOnlyException() throw() {}
} ;

class FounderXDB_EXPORT CollectionNotExistException : public virtual ExecutionEngineException
{
    //CollectionNotExistException (const CollectionNotExistException &);
    CollectionNotExistException & operator = (const CollectionNotExistException&);
    const static ErrorCode_t error_code  =  ERROR_COLLECTION_NOT_EXIST;
public:
    CollectionNotExistException ();
    explicit CollectionNotExistException (const char* msg);
    virtual  ~CollectionNotExistException() throw() {}
} ;

class FounderXDB_EXPORT CollectionAlreadyExistException : public virtual ExecutionEngineException
{
    //CollectionAlreadyExistException (const CollectionAlreadyExistException &);
    CollectionAlreadyExistException & operator = (const CollectionAlreadyExistException&);
    const static ErrorCode_t error_code  =  ERROR_COLLECTION_ALREADY_EXIST;
public:
    CollectionAlreadyExistException ();
    explicit CollectionAlreadyExistException (const char* msg);
    virtual  ~CollectionAlreadyExistException() throw() {}
} ;


class FounderXDB_EXPORT DocFileNotExistException : public virtual ExecutionEngineException
{
    //DocFileNotExistException (const DocFileNotExistException &);
    DocFileNotExistException & operator = (const DocFileNotExistException&);
    const static ErrorCode_t error_code  =  ERROR_DOCFILE_NOT_EXIST;
public:
    DocFileNotExistException ();
    explicit DocFileNotExistException (const char* msg);
    virtual  ~DocFileNotExistException() throw() {}
} ;

class FounderXDB_EXPORT ObjectAlreadyExistException : public virtual ExecutionEngineException
{
    ObjectAlreadyExistException & operator = (const ObjectAlreadyExistException&);
    const static ErrorCode_t error_code  =  ERR_OBJECT_ALREADY_EXIST  ;
public:
    ObjectAlreadyExistException ();
    explicit ObjectAlreadyExistException (const char* msg);
    virtual  ~ObjectAlreadyExistException() throw() {}
} ;

class FounderXDB_EXPORT ObjectNotExistException : public virtual ExecutionEngineException
{
    ObjectNotExistException & operator = (const ObjectNotExistException&);
    const static ErrorCode_t error_code  =  ERR_OBJECT_NOT_EXIST  ;
public:
    ObjectNotExistException ();
    explicit ObjectNotExistException (const char* msg);
    virtual  ~ObjectNotExistException() throw() {}
} ;
    
class FounderXDB_EXPORT CheckDocItemCascadeException : public virtual ExecutionEngineException
{
    //DocFileNotExistException (const DocFileNotExistException &);
    CheckDocItemCascadeException & operator = (const CheckDocItemCascadeException&);
    const static ErrorCode_t error_code  =  ERROR_CHECK_DOCITEM_CASCADE;
public:
    CheckDocItemCascadeException ();
    explicit CheckDocItemCascadeException (const char* msg);
    virtual  ~CheckDocItemCascadeException() throw() {}
} ;

class FounderXDB_EXPORT DocFileAlreadyExistException : public virtual ExecutionEngineException
{
    //DocFileAlreadyExistException (const DocFileAlreadyExistException &);
    DocFileAlreadyExistException & operator = (const DocFileNotExistException&);
    const static ErrorCode_t error_code  =  ERROR_DOCFILE_ALREADY_EXIST;
public:
    DocFileAlreadyExistException ();
    explicit DocFileAlreadyExistException (const char* msg);
    virtual  ~DocFileAlreadyExistException() throw() {}
} ;

class FounderXDB_EXPORT UserAlreadyExistException : public virtual ExecutionEngineException
{
    //UserAlreadyExistException (const UserAlreadyExistException &);
    UserAlreadyExistException & operator = (const UserAlreadyExistException&);
    const static ErrorCode_t error_code  =  ERR_USER_ALREADY_EXIST;
public:
    UserAlreadyExistException ();
    explicit UserAlreadyExistException (const char* msg);
    virtual  ~UserAlreadyExistException() throw() {}
} ;

//the exception 'ERR_USER_NOT_EXIST'.
class FounderXDB_EXPORT UserNotExistException : public virtual ExecutionEngineException
{
    //UserNotExistException (const UserNotExistException &);
    UserNotExistException & operator = (const UserNotExistException&);
    const static ErrorCode_t error_code  =  ERR_USER_NOT_EXIST;
public:
    UserNotExistException ();
    explicit UserNotExistException (const char* msg);
    virtual  ~UserNotExistException() throw() {}
} ;

//the exception for 'ERR_PASSWORD_INCORRECT'.
class FounderXDB_EXPORT PasswordIncorrectException : public virtual ExecutionEngineException
{
    //PasswordIncorrect (const PasswordIncorrect &);
    PasswordIncorrectException & operator = (const PasswordIncorrectException&);
    const static ErrorCode_t error_code  =  ERR_PASSWORD_INCORRECT;
public:
    PasswordIncorrectException ();
    explicit PasswordIncorrectException (const char* msg);
    virtual  ~PasswordIncorrectException() throw() {}
} ;

class FounderXDB_EXPORT UserRoleIsNotAdminException : public virtual ExecutionEngineException
{
    UserRoleIsNotAdminException & operator = (const UserRoleIsNotAdminException&);
    const static ErrorCode_t error_code  =  ERR_USEROLE_IS_NOT_ADMIN;
public:
    UserRoleIsNotAdminException ();
    explicit UserRoleIsNotAdminException (const char* msg);
    virtual  ~UserRoleIsNotAdminException() throw() {}
} ;

class FounderXDB_EXPORT SequenceErroException : public virtual ExecutionEngineException
{
    SequenceErroException & operator = (const SequenceErroException&);
    const static ErrorCode_t error_code  =  ERR_SEQUENCE_ERROR;
public:
    SequenceErroException ();
    explicit SequenceErroException (const char* msg);
    virtual  ~SequenceErroException() throw() {}
} ;

//the exception for "NewPasswdSameWithOldPasswd"
class FounderXDB_EXPORT NewPasswdSameWithOldPasswdException : public virtual ExecutionEngineException
{
	//NewPasswdSameWithOldPasswdException (const NewPasswdSameWithOldPasswdException &);
	NewPasswdSameWithOldPasswdException & operator = (const NewPasswdSameWithOldPasswdException&);
	const static ErrorCode_t error_code  =  ERR_NEW_PASSWORD_SAME_WITH_OLD_PASSWORD;
public:
	NewPasswdSameWithOldPasswdException ();
	explicit NewPasswdSameWithOldPasswdException (const char* msg);
	virtual  ~NewPasswdSameWithOldPasswdException() throw() {}
} ;

class FounderXDB_EXPORT PreparedStatementNotExist : public virtual ExecutionEngineException
{
    //PasswordIncorrect (const PasswordIncorrect &);
    PreparedStatementNotExist & operator = (const PreparedStatementNotExist&);
    const static ErrorCode_t error_code  =  ERR_PREPARED_STATEMENT_NOT_EXIST;
public:
    PreparedStatementNotExist ();
    explicit PreparedStatementNotExist (const char* msg);
    virtual  ~PreparedStatementNotExist() throw() {}
} ;

class FounderXDB_EXPORT RoleAlreadyExistException : public virtual ExecutionEngineException
{
    //RoleAlreadyExistException (const RoleAlreadyExistException &);
    RoleAlreadyExistException & operator = (const RoleAlreadyExistException&);
    const static ErrorCode_t error_code  =  ERROR_ROLE_ALREADY_EXIST;
public:
    RoleAlreadyExistException ();
    explicit RoleAlreadyExistException (const char* msg);
    virtual  ~RoleAlreadyExistException() throw() {}
} ;

//the exception for 'ERROR_ROLE_NOT_EXIST'.
class FounderXDB_EXPORT RoleNotExistException : public virtual ExecutionEngineException
{
    //RoleNotExistException (const RoleNotExistException &);
    RoleNotExistException & operator = (const RoleNotExistException&);
    const static ErrorCode_t error_code  =ERROR_ROLE_NOT_EXIST;
public:
    RoleNotExistException ();
    explicit RoleNotExistException (const char* msg);
    virtual  ~RoleNotExistException() throw() {}
} ;

//DROP USER EXCEPTION
//the exception for 'ERROR_CAN_NOT_BIND_USER_NAME'.
class FounderXDB_EXPORT CanNotBindUserNameException : public virtual ExecutionEngineException
{
    //CanNotBindUserNameException (const CanNotBindUserNameException &);
    CanNotBindUserNameException & operator = (const CanNotBindUserNameException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_BIND_USER_NAME;
public:
    CanNotBindUserNameException ();
    explicit CanNotBindUserNameException (const char* msg);
    virtual  ~CanNotBindUserNameException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER'.
class FounderXDB_EXPORT CanNotDeleteSystemTableUserException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTableUserException (const CanNotDeleteSystemTableUserException &);
    CanNotDeleteSystemTableUserException & operator = (const CanNotDeleteSystemTableUserException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER;
public:
    CanNotDeleteSystemTableUserException ();
    explicit CanNotDeleteSystemTableUserException (const char* msg);
    virtual  ~CanNotDeleteSystemTableUserException() throw() {}
} ;

class FounderXDB_EXPORT CanNotDeleteSystemTableException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTableException (const CanNotDeleteSystemTableException &);
    CanNotDeleteSystemTableException & operator = (const CanNotDeleteSystemTableException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_SYSTEM_TABLE;
public:
    CanNotDeleteSystemTableException ();
    explicit CanNotDeleteSystemTableException (const char* msg);
    virtual  ~CanNotDeleteSystemTableException() throw() {}
} ;

class FounderXDB_EXPORT CanNotInsertSystemTableException : public virtual ExecutionEngineException
{
    //CanNotInsertSystemTableException (const CanNotInsertSystemTableException &);
    CanNotInsertSystemTableException & operator = (const CanNotInsertSystemTableException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_INSERT_SYSTEM_TABLE;
public:
    CanNotInsertSystemTableException ();
    explicit CanNotInsertSystemTableException (const char* msg);
    virtual  ~CanNotInsertSystemTableException() throw() {}
} ;

class FounderXDB_EXPORT CanNotInsertSystemTreeTableException : public virtual ExecutionEngineException
{
    //CanNotInsertSystemTreeTableException (const CanNotInsertSystemTreeTableException &);
    CanNotDeleteSystemTableException & operator = (const CanNotDeleteSystemTableException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_INSERT_SYSTEM_TREE_TABLE;
public:
    CanNotInsertSystemTreeTableException ();
    explicit CanNotInsertSystemTreeTableException (const char* msg);
    virtual  ~CanNotInsertSystemTreeTableException() throw() {}
} ;

class FounderXDB_EXPORT CanNotDeleteSystemTreeTableException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTreeTableException (const CanNotDeleteSystemTreeTableException &);
    CanNotDeleteSystemTreeTableException & operator = (const CanNotDeleteSystemTableException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_SYSTEM_TREE_TABLE;
public:
    CanNotDeleteSystemTreeTableException ();
    explicit CanNotDeleteSystemTreeTableException (const char* msg);
    virtual  ~CanNotDeleteSystemTreeTableException() throw() {}
} ;


class FounderXDB_EXPORT GetCurrentDatabaseException : public virtual ExecutionEngineException
{
    //GetCurrentDatabaseException (const CanNotDeleteSystemTreeTableException &);
    GetCurrentDatabaseException & operator = (const GetCurrentDatabaseException&);
    const static ErrorCode_t error_code  =  ERROR_GET_CURRENT_DATABASE;
public:
    GetCurrentDatabaseException ();
    explicit GetCurrentDatabaseException (const char* msg);
    virtual  ~GetCurrentDatabaseException() throw() {}
} ;


class FounderXDB_EXPORT GetCurrentUserException : public virtual ExecutionEngineException
{
    //GetCurrentUserException (const CanNotDeleteSystemTreeTableException &);
    GetCurrentUserException & operator = (const GetCurrentUserException&);
    const static ErrorCode_t error_code  =  ERROR_GET_CURRENT_USER;
public:
    GetCurrentUserException ();
    explicit GetCurrentUserException (const char* msg);
    virtual  ~GetCurrentUserException() throw() {}
} ;


class FounderXDB_EXPORT CheckItemRelationException : public virtual ExecutionEngineException
{
    //CheckItemRelationException (const CanNotDeleteSystemTreeTableException &);
    CheckItemRelationException & operator = (const GetCurrentUserException&);
    const static ErrorCode_t error_code  =  ERROR_CHECK_ITEM_RELATION;
public:
    CheckItemRelationException ();
    explicit CheckItemRelationException (const char* msg);
    virtual  ~CheckItemRelationException() throw() {}
} ;
class FounderXDB_EXPORT RelationShipNotExistException : public virtual ExecutionEngineException
{
    //RelationShipNotExistException (const RelationShipNotExistException &);
    RelationShipNotExistException & operator = (const RelationShipNotExistException&);
    const static ErrorCode_t error_code  =  ERROR_RELATIONSHIP_NOT_EXIST;
public:
    RelationShipNotExistException ();
    explicit RelationShipNotExistException (const char* msg);
    virtual  ~RelationShipNotExistException() throw() {}
} ;

class FounderXDB_EXPORT RelationShipHasExistException : public virtual ExecutionEngineException
{
    //RelationShipHasExistException (const RelationShipHastExistException &);
    RelationShipHasExistException & operator = (const RelationShipHasExistException&);
    const static ErrorCode_t error_code  =  ERROR_DOCITEM_RELATION_HAS_EXIST;
public:
    RelationShipHasExistException ();
    explicit RelationShipHasExistException (const char* msg);
    virtual  ~RelationShipHasExistException() throw() {}
} ;

class FounderXDB_EXPORT CanNotBindUserIDException : public virtual ExecutionEngineException
{
    //CanNotBindUserIDException (const CanNotBindUserIDException &);
    CanNotBindUserIDException & operator = (const CanNotBindUserIDException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_BIND_USER_ID;
public:
    CanNotBindUserIDException ();
    explicit CanNotBindUserIDException (const char* msg);
    virtual  ~CanNotBindUserIDException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER_PRIVILEGE'.
class FounderXDB_EXPORT CanNotDeleteSystemTableUserPrivilegeException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTableUserPrivilegeException (const CanNotDeleteSystemTableUserPrivilegeException &);
    CanNotDeleteSystemTableUserPrivilegeException & operator = (const CanNotDeleteSystemTableUserPrivilegeException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER_PRIVILEGE;
public:
    CanNotDeleteSystemTableUserPrivilegeException ();
    explicit CanNotDeleteSystemTableUserPrivilegeException (const char* msg);
    virtual  ~CanNotDeleteSystemTableUserPrivilegeException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_BIND_USER_ID_FROM_ROLE_USER'.
class FounderXDB_EXPORT CanNotBindUserIDFromRoleUserException : public virtual ExecutionEngineException
{
    //CanNotBindUserIDFromRoleUserException (const CanNotBindUserIDFromRoleUserException &);
    CanNotBindUserIDFromRoleUserException & operator = (const CanNotBindUserIDFromRoleUserException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_BIND_USER_ID_FROM_ROLE_USER;
public:
    CanNotBindUserIDFromRoleUserException ();
    explicit CanNotBindUserIDFromRoleUserException (const char* msg);
    virtual  ~CanNotBindUserIDFromRoleUserException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_USER_ID'.
class FounderXDB_EXPORT CanNotDeleteSystemTableRoleUserByUserIDException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTableRoleUserByUserIDException (const CanNotDeleteSystemTableRoleUserByUserIDException &);
    CanNotDeleteSystemTableRoleUserByUserIDException & operator = (const CanNotDeleteSystemTableRoleUserByUserIDException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_USER_ID;
public:
    CanNotDeleteSystemTableRoleUserByUserIDException ();
    explicit CanNotDeleteSystemTableRoleUserByUserIDException (const char* msg);
    virtual  ~CanNotDeleteSystemTableRoleUserByUserIDException() throw() {}
} ;

///DROP ROLE CXCEPTION
//the exception for 'ERROR_CAN_NOT_BIND_ROLE_NAME'.
class FounderXDB_EXPORT CanNotBindRoleNameException : public virtual ExecutionEngineException
{
    //CanNotBindRoleNameException (const CanNotBindRoleNameException &);
    CanNotBindRoleNameException & operator = (const CanNotBindRoleNameException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_BIND_ROLE_NAME;
public:
    CanNotBindRoleNameException ();
    explicit CanNotBindRoleNameException (const char* msg);
    virtual  ~CanNotBindRoleNameException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE'.
class FounderXDB_EXPORT CanNotDeleteSystemTableRoleException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTableRoleException (const CanNotDeleteSystemTableRoleException &);
    CanNotDeleteSystemTableRoleException & operator = (const CanNotDeleteSystemTableRoleException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE;
public:
    CanNotDeleteSystemTableRoleException ();
    explicit CanNotDeleteSystemTableRoleException (const char* msg);
    virtual  ~CanNotDeleteSystemTableRoleException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_BIND_ROLE_ID'.
class FounderXDB_EXPORT CanNotBindRoleIDException : public virtual ExecutionEngineException
{
    //CanNotBindRoleIDException (const CanNotBindRoleIDException &);
    CanNotBindRoleIDException & operator = (const CanNotBindRoleIDException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_BIND_ROLE_ID;
public:
    CanNotBindRoleIDException ();
    explicit CanNotBindRoleIDException (const char* msg);
    virtual  ~CanNotBindRoleIDException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE_PRIVILEGE'.
class FounderXDB_EXPORT CanNotDeleteSystemTableRolePrivilegeException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTableRolePrivilegeException (const CanNotDeleteSystemTableRolePrivilegeException &);
    CanNotDeleteSystemTableRolePrivilegeException & operator = (const CanNotDeleteSystemTableRolePrivilegeException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE_PRIVILEGE;
public:
    CanNotDeleteSystemTableRolePrivilegeException ();
    explicit CanNotDeleteSystemTableRolePrivilegeException (const char* msg);
    virtual  ~CanNotDeleteSystemTableRolePrivilegeException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_BIND_ROLE_ID_FROM_ROLE_USER'.
class FounderXDB_EXPORT CanNotBindRoleIDFromRoleUserException : public virtual ExecutionEngineException
{
    //CanNotBindRoleIDFromRoleUserException (const CanNotBindRoleIDFromRoleUserException &);
    CanNotBindRoleIDFromRoleUserException & operator = (const CanNotBindRoleIDFromRoleUserException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_BIND_ROLE_ID_FROM_ROLE_USER;
public:
    CanNotBindRoleIDFromRoleUserException ();
    explicit CanNotBindRoleIDFromRoleUserException (const char* msg);
    virtual  ~CanNotBindRoleIDFromRoleUserException() throw() {}
} ;

//the exception for 'ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_ROLE_ID'.
class FounderXDB_EXPORT CanNotDeleteSystemTableRoleUserByRoleIDException : public virtual ExecutionEngineException
{
    //CanNotDeleteSystemTableRoleUserByRoleIDException (const CanNotDeleteSystemTableRoleUserByRoleIDException &);
    CanNotDeleteSystemTableRoleUserByRoleIDException & operator = (const CanNotDeleteSystemTableRoleUserByRoleIDException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_ROLE_ID;
public:
    CanNotDeleteSystemTableRoleUserByRoleIDException ();
    explicit CanNotDeleteSystemTableRoleUserByRoleIDException (const char* msg);
    virtual  ~CanNotDeleteSystemTableRoleUserByRoleIDException() throw() {}
} ;

class FounderXDB_EXPORT UserOrRoleNotExistException : public virtual ExecutionEngineException
{
    //UserOrRoleNotExistException (const UserOrRoleNotExistException &);
    UserOrRoleNotExistException & operator = (const UserOrRoleNotExistException&);
    const static ErrorCode_t error_code  =  ERROR_USER_OR_ROLE_NOT_EXIST;
public:
    UserOrRoleNotExistException ();
    explicit UserOrRoleNotExistException (const char* msg);
    virtual  ~UserOrRoleNotExistException() throw() {}
} ;

class FounderXDB_EXPORT ObjPrivsNotGrantedException : public virtual ExecutionEngineException
{
    //ObjPrivsNotGrantedException (const ObjPrivsNotGrantedException &);
    ObjPrivsNotGrantedException & operator = (const ObjPrivsNotGrantedException&);
    const static ErrorCode_t error_code  =  ERROR_OBJECT_PRIVILEGE_NOT_GRANTED;
public:
    ObjPrivsNotGrantedException ();
    explicit ObjPrivsNotGrantedException (const char* msg);
    virtual  ~ObjPrivsNotGrantedException() throw() {}
} ;

class FounderXDB_EXPORT SysPrivsNotGrantedException : public virtual ExecutionEngineException
{
    //SysPrivsNotGrantedException (const SysPrivsNotGrantedException &);
    SysPrivsNotGrantedException & operator = (const SysPrivsNotGrantedException&);
    const static ErrorCode_t error_code  =  ERROR_SYS_PRIVILEGE_NOT_GRANTED;
public:
    SysPrivsNotGrantedException ();
    explicit SysPrivsNotGrantedException (const char* msg);
    virtual  ~SysPrivsNotGrantedException() throw() {}
} ;

class FounderXDB_EXPORT DeleteCollectionException : public virtual ExecutionEngineException
{
    //DeleteCollectionException (const DeleteCollectionException &);
    DeleteCollectionException & operator = (const DeleteCollectionException&);
    const static ErrorCode_t error_code  =  ERROR_DELETE_COLLECTION;
public:
    DeleteCollectionException ();
    explicit DeleteCollectionException (const char* msg);
    virtual  ~DeleteCollectionException() throw() {}
} ;

class FounderXDB_EXPORT ResourceNotFoundException : public virtual ExecutionEngineException
{
    //ResourceNotFoundException (const ResourceNotFoundException &);
    ResourceNotFoundException & operator = (const ResourceNotFoundException&);
    const static ErrorCode_t error_code  =  ERR_RESOURCE_NOT_FOUND;
public:
    ResourceNotFoundException ();
    explicit ResourceNotFoundException (const char* msg);
    virtual  ~ResourceNotFoundException() throw() {}
} ;

class FounderXDB_EXPORT PrivNotExistException : public virtual ExecutionEngineException
{
    //PrivNotExistException (const PrivNotExistException &);
    PrivNotExistException & operator = (const PrivNotExistException&);
    const static ErrorCode_t error_code  =  ERROR_PRIVILEGE_NOT_EXIST;
public:
    PrivNotExistException ();
    explicit PrivNotExistException (const char* msg);
    virtual  ~PrivNotExistException() throw() {}
} ;

class FounderXDB_EXPORT UserNotAdminException : public virtual ExecutionEngineException
{
    //UserNotAdminException (const UserNotAdminException &);
    UserNotAdminException & operator = (const UserNotAdminException&);
    const static ErrorCode_t error_code  =  ERROR_USER_NOT_ADMIN;
public:
    UserNotAdminException ();
    explicit UserNotAdminException (const char* msg);
    virtual  ~UserNotAdminException() throw() {}
} ;

class FounderXDB_EXPORT PathNotExistException : public virtual ExecutionEngineException
{
    //PathNotExistException (const PathNotExistException &);
    PathNotExistException & operator = (const PathNotExistException&);
    const static ErrorCode_t error_code  =  ERROR_PATH_NOT_EXIST;
public:
    PathNotExistException ();
    explicit PathNotExistException (const char* msg);
    virtual  ~PathNotExistException() throw() {}
} ;

class FounderXDB_EXPORT PathAlreadyExistException : public virtual ExecutionEngineException
{
    //PathNotExistException (const PathAlreadyExistException &);
    PathAlreadyExistException & operator = (const PathAlreadyExistException&);
    const static ErrorCode_t error_code  =  ERROR_PATH_ALREADY_EXIST;
public:
    PathAlreadyExistException ();
    explicit PathAlreadyExistException (const char* msg);
    virtual  ~PathAlreadyExistException() throw() {}
} ;

class FounderXDB_EXPORT CanNotChangeAdminUserException : public virtual ExecutionEngineException
{
    //CanNotChangeAdminUserException (const CanNotChangeAdminUserException &);
    CanNotChangeAdminUserException & operator = (const CanNotChangeAdminUserException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_CHANGE_ADMIN_USER;
public:
    CanNotChangeAdminUserException ();
    explicit CanNotChangeAdminUserException (const char* msg);
    virtual  ~CanNotChangeAdminUserException() throw() {}
} ;

class FounderXDB_EXPORT CanNotChangeSysAdminRoleException : public virtual ExecutionEngineException
{
    //CanNotChangeSysAdminRoleException (const CanNotChangeSysAdminRoleException &);
    CanNotChangeSysAdminRoleException & operator = (const CanNotChangeSysAdminRoleException&);
    const static ErrorCode_t error_code  =  ERROR_CAN_NOT_CHANGE_SYS_ADMIN_ROLE;
public:
    CanNotChangeSysAdminRoleException ();
    explicit CanNotChangeSysAdminRoleException (const char* msg);
    virtual  ~CanNotChangeSysAdminRoleException() throw() {}
} ;


class FounderXDB_EXPORT InternalQueryException : public virtual ExecutionEngineException
{
    //CanNotChangeSysAdminRoleException (const CanNotChangeSysAdminRoleException &);
    InternalQueryException  & operator = (const InternalQueryException &);
    const static ErrorCode_t error_code  =  ERR_INTERNAL_QUERY;
public:
    InternalQueryException ();
    explicit InternalQueryException (const char* msg);
    virtual  ~InternalQueryException() throw() {}
} ;

//the namelegal exception
class FounderXDB_EXPORT NameLegalException : public virtual ExecutionEngineException
{
    NameLegalException  & operator = (const NameLegalException &);
    const static ErrorCode_t error_code  =  ERR_NAME_LEGAL;
public:
    NameLegalException ();
    explicit NameLegalException (const char* msg);
    virtual  ~NameLegalException() throw() {}
} ;

//DatabaseException
//the exception for 'ERR_DB_USER_EXISTS'.
class FounderXDB_EXPORT UserHasExistException : public virtual DatabaseException
{
    //UserHasExistsException (const UserHasExistsException&);                     
    UserHasExistException& operator = (const UserHasExistException&);
    const static ErrorCode_t error_code  = ERR_DB_USER_EXISTS ;
public:
    UserHasExistException ();
    explicit UserHasExistException (const char* msg);
    virtual ~UserHasExistException () throw() {}
} ;   

//throws an exception, UserNotExistsException, when performs an operation on database user, or else.  
class FounderXDB_EXPORT UserNotExistsException : public virtual DatabaseException
{
    //UserNotExistsException (const UserNotExistsException&) ;
    UserNotExistsException& operator = (const UserNotExistsException&) ;
    const static ErrorCode_t error_code = ERR_DB_USER_NOT_EXISTS ; 
public:
    UserNotExistsException () ;
    explicit UserNotExistsException (const char* msg) ;
    virtual ~UserNotExistsException() throw() {}
} ;

class FounderXDB_EXPORT TablespaceNotExistsException : public virtual DatabaseException
{
	TablespaceNotExistsException& operator = (const TablespaceNotExistsException&) ;
	const static ErrorCode_t error_code = ERR_STORAGE_OBJECT_NOT_EXIST ; 
public:
	TablespaceNotExistsException () ;
	explicit TablespaceNotExistsException (const char* msg) ;
	virtual ~TablespaceNotExistsException() throw() {}
} ;
class FounderXDB_EXPORT TablespaceRemoveException : public virtual DatabaseException
{
    TablespaceRemoveException& operator = (const TablespaceRemoveException&) ;
    const static ErrorCode_t error_code = ERR_STORAGE_TABLESPACE_REMOVE ; 
public:
    TablespaceRemoveException () ;
    explicit TablespaceRemoveException (const char* msg) ;
    virtual ~TablespaceRemoveException() throw() {}
} ;

class FounderXDB_EXPORT ExecuteBackupException : public virtual StorageEngineException
{
    ExecuteBackupException& operator = (const ExecuteBackupException&) ;
    const static ErrorCode_t error_code = ERR_STORAGE_EXECUTE_BACKUP ; 
public:
    ExecuteBackupException () ;
    explicit ExecuteBackupException (const char* msg) ;
    virtual ~ExecuteBackupException() throw() {}
} ;

class FounderXDB_EXPORT ExecuteRestoreException : public virtual StorageEngineException
{
    ExecuteRestoreException& operator = (const ExecuteRestoreException&) ;
    const static ErrorCode_t error_code = ERR_STORAGE_EXECUTE_RESTORE ; 
public:
    ExecuteRestoreException () ;
    explicit ExecuteRestoreException (const char* msg) ;
    virtual ~ExecuteRestoreException() throw() {}
} ;

class FounderXDB_EXPORT CheckSystemStatusException : public virtual StorageEngineException
{
	CheckSystemStatusException& operator = (const CheckSystemStatusException&) ;
	const static ErrorCode_t error_code = ERR_STORAGE_SYSTEM_STATUS ; 
public:
	CheckSystemStatusException () ;
	explicit CheckSystemStatusException (const char* msg) ;
	virtual ~CheckSystemStatusException() throw() {}
} ;

//throw a RoleHasExistException when does operation on users' roles. Such as, create a role or drop a role.
class FounderXDB_EXPORT RoleHasExistException : public virtual DatabaseException
{
    //RoleHasExistException (const RoleHasExistException &) ;
    RoleHasExistException& operator = (const RoleHasExistException&) ;
    const static ErrorCode_t error_code  = ERR_DB_ROLE_EXISTS ; 
public: 
    RoleHasExistException () ;
    explicit RoleHasExistException (const char* msg) ;
    virtual ~RoleHasExistException () throw() {}
};

//throws a RoleNotExistsException when does operation on users' roles. Such as, create a role or drop a role.
class FounderXDB_EXPORT RoleNotExistsException : public virtual DatabaseException
{
    //RoleNotExistsException (const RoleNotExistsException& );
    RoleNotExistsException& operator = (const RoleNotExistsException&);
    const static ErrorCode_t error_code  = ERR_DB_ROLE_NOT_EXISTS ; 
public :
    RoleNotExistsException ();
    explicit RoleNotExistsException (const char* msg);
    virtual ~RoleNotExistsException () throw() {}
} ;

//throws this exception when the user is an un-authorized user.
class FounderXDB_EXPORT NotAuthorizedException : public virtual DatabaseException
{
    //NotAuthorizedException (const NotAuthorizedException& );
    NotAuthorizedException& operator = (const NotAuthorizedException&);
    const static ErrorCode_t error_code  = ERR_DB_NOT_AUTHORIZED ; 
public :

    NotAuthorizedException ();
    explicit NotAuthorizedException (const char* msg);
    virtual ~NotAuthorizedException () throw() {} 
} ;

class FounderXDB_EXPORT DatabaseNotUsedException : public virtual DatabaseException
{
    //NotAuthorizedException (const NotAuthorizedException& );
    DatabaseNotUsedException& operator = (const DatabaseNotUsedException&);
    const static ErrorCode_t error_code  = ERR_DATABASE_NOT_USED ; 
public :

    DatabaseNotUsedException ();
    explicit DatabaseNotUsedException (const char* msg);
    virtual ~DatabaseNotUsedException () throw() {} 
} ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FounderXDB_EXPORT AddressInUseException : public virtual ServerFrameworkException
{
    //NotAuthorizedException (const NotAuthorizedException& );
    AddressInUseException& operator = (const AddressInUseException&);
    const static ErrorCode_t error_code  = ERR_NET_ADDRESS_IN_USE ; 
public :
    AddressInUseException (const char* ipstr, const int& port);
    explicit AddressInUseException (const char* msg);
    virtual ~AddressInUseException () throw() {} 
} ;

class FounderXDB_EXPORT OpenFileFailedException : public virtual OtherBaseException
{
	OpenFileFailedException& operator = (const OpenFileFailedException&);
	const static ErrorCode_t error_code  = ERR_FILE_OPEN_FAILED ; 
public :
	OpenFileFailedException ();
	explicit OpenFileFailedException (const char* msg);
	virtual ~OpenFileFailedException () throw() {} 
} ;

class FounderXDB_EXPORT DocContentException : public virtual OtherBaseException
{
    DocContentException& operator = (const DocContentException&);
    const static ErrorCode_t error_code  = ERR_DOC_CONTENT ; 
public :
    DocContentException ();
    explicit DocContentException (const char* msg);
    virtual ~DocContentException () throw() {} 
} ;

class FounderXDB_EXPORT FileEmptyException : public virtual OtherBaseException
{
	FileEmptyException& operator = (const FileEmptyException&);
	const static ErrorCode_t error_code  = ERR_FILE_EMPTY ; 
public :
	FileEmptyException ();
	explicit FileEmptyException (const char* msg);
	virtual ~FileEmptyException () throw() {} 
} ;

class FounderXDB_EXPORT FileTooLargeException : public virtual OtherBaseException
{
	FileTooLargeException& operator = (const FileTooLargeException&);
	const static ErrorCode_t error_code  = ERR_FILE_TOO_LARGE ; 
public :
	FileTooLargeException ();
	explicit FileTooLargeException (const char* msg);
	virtual ~FileTooLargeException () throw() {} 
} ;


class FounderXDB_EXPORT ConversionFailedException : public virtual OtherBaseException
{
    ConversionFailedException& operator = (const ConversionFailedException) ;
    const static ErrorCode_t error_code  =  ERR_CONVERSION_FAILED ;
public :
    ConversionFailedException () ;
    explicit  ConversionFailedException (const char* msg) ;
    virtual ~ConversionFailedException () throw () {}
} ;

///FullText Exception
//throw this exception, when the resource is not found in fulltext
class FounderXDB_EXPORT ResourceNotFoundExceptionFulltext : public virtual FullTextException
{
    //ResourceNotFoundException (const RescourceNotFoundException &);
    ResourceNotFoundExceptionFulltext & operator = (const ResourceNotFoundExceptionFulltext&);
    const static ErrorCode_t error_code = ERR_RESOURCE_NOT_FOUND_FULLTEXT;
public:
    ResourceNotFoundExceptionFulltext ();
    explicit ResourceNotFoundExceptionFulltext (const char* msg);
    virtual ~ResourceNotFoundExceptionFulltext() throw() {}
};

// It is a static error if, during the static analysis phase, 
// the query is found to contain a stop word option that refers to a stop word list 
// that is not found in the statically known stop word lists.
class FounderXDB_EXPORT StopwordNotFoundException : public virtual FullTextException
{
    //StopwordNotFoundException (const StopwordNotFoundException &);
    StopwordNotFoundException & operator = (const StopwordNotFoundException&);
    const static ErrorCode_t error_code = ERR_STOPWORD_NOTFOUND;
public:
    StopwordNotFoundException ();
    explicit StopwordNotFoundException (const char* msg);
    virtual ~StopwordNotFoundException() throw() {}
};

// An implementation may restrict the use of FTWindow and FTDistance to an FTOr that is 
// either a single FTWords or a combination of FTWords involving only the operators ftand and ftor.
// If it a static error if, during the static analysis phase,
// an expression is found that violates this restriction and the implementation enforces this restriction.
class FounderXDB_EXPORT FTWindowAndFTDistanceException : public virtual FullTextException
{
    //FTWindowAndFTDistanceException (const FTWindowAndFTDistanceException &);
    FTWindowAndFTDistanceException & operator = (const FTWindowAndFTDistanceException&);
    const static ErrorCode_t error_code = ERR_FTWINDOW_AND_FTDISTANCE;
public:
    FTWindowAndFTDistanceException ();
    explicit FTWindowAndFTDistanceException (const char* msg);
    virtual ~FTWindowAndFTDistanceException() throw() {}
};

//An implementation that does not support the FTContent operator must raise a static error if a full-text query contains one.
class FounderXDB_EXPORT QueryContainsFtcontentException : public virtual FullTextException
{
    //QueryContainsFtcontentException (const QueryContainsFtcontentException &);
    QueryContainsFtcontentException & operator = (const QueryContainsFtcontentException&);
    const static ErrorCode_t error_code = ERR_QUERY_CONTAINS_FTCONTENT;
public:
    QueryContainsFtcontentException ();
    explicit QueryContainsFtcontentException (const char* msg);
    virtual ~QueryContainsFtcontentException() throw() {}
};

//*It is a static error if, during the static analysis phase, an implementation that restricts the use of FTLanguageOption to a single language, encounters more than one distinct language option. */
class FounderXDB_EXPORT FTLanguageOptionsException : public virtual FullTextException
{
    //FTLanguageOptionsException (const FTLanguageOptionsException &);
    FTLanguageOptionsException & operator = (const FTLanguageOptionsException&);
    const static ErrorCode_t error_code = ERR_FTLANGUAGE_OPTIONS;
public:
    FTLanguageOptionsException ();
    explicit FTLanguageOptionsException (const char* msg);
    virtual ~FTLanguageOptionsException() throw() {}
};

// It is a static error if, during the static analysis phase, an implementation that restricts the choices of FTCaseOption encounters the "lowercase" or "uppercase" option.
class FounderXDB_EXPORT FTCaseOptionException : public virtual FullTextException
{
    //FTCaseOptionException (const FTCaseOptionException &);
    FTCaseOptionException & operator = (const FTCaseOptionException&);
    const static ErrorCode_t error_code = ERR_FTCASE_OPTION_EXCEPTION;
public:
    FTCaseOptionException ();
    explicit FTCaseOptionException (const char* msg);
    virtual ~FTCaseOptionException() throw() {}
};

// It is a dynamic error if a weight value is not within the required range of values; it is also a dynamic error if an implementation that does not support negative weights encounters a negative weight value.
class FounderXDB_EXPORT WeightValueException : public virtual FullTextException
{
    //WeightValueException (const WeightValueException &);
    WeightValueException & operator = (const WeightValueException&);
    const static ErrorCode_t error_code = ERR_WEIGHT_VALUE;
public:
    WeightValueException ();
    explicit WeightValueException (const char* msg);
    virtual ~WeightValueException() throw() {}
};

// It is a dynamic error if an implementation encounters a mild not selection, one of whose operands evaluates to an AllMatches that contains a StringExclude.
class FounderXDB_EXPORT MildNotSelectionException : public virtual FullTextException
{
    //MildNotSelectionException (const MildNotSelectionException &);
    MildNotSelectionException & operator = (const MildNotSelectionException&);
    const static ErrorCode_t error_code = ERR_MILD_NOT_SELECTION;
public:
    MildNotSelectionException ();
    explicit MildNotSelectionException (const char* msg);
    virtual ~MildNotSelectionException() throw() {}
};

// It is a static error if, within a single FTMatchOptions, there is more than one match option of any given match option group.
class FounderXDB_EXPORT FTMatchOptionsExceptions : public virtual FullTextException
{
    //FTMatchOptionsExceptions (const FTMatchOptionsExceptions &);
    FTMatchOptionsExceptions & operator = (const FTMatchOptionsExceptions&);
    const static ErrorCode_t error_code = ERR_FTMATCH_OPTIONS;
public:
    FTMatchOptionsExceptions ();
    explicit FTMatchOptionsExceptions (const char* msg);
    virtual ~FTMatchOptionsExceptions() throw() {}
};

// It is a dynamic error if, when "wildcards" is in effect, a query string violates wildcard syntax.
class FounderXDB_EXPORT WildcardSyntaxException : public virtual FullTextException
{
    //WildcardSyntaxException (const WildcardSyntaxException &);
    WildcardSyntaxException & operator = (const WildcardSyntaxException&);
    const static ErrorCode_t error_code = ERR_WILDCARD_SYNTAX;
public:
    WildcardSyntaxException ();
    explicit WildcardSyntaxException (const char* msg);
    virtual ~WildcardSyntaxException() throw() {}
};

// It is a IKAnalyzer error if, when "IKAnalyzer" encounters errors.
class FounderXDB_EXPORT IKAnalyzerException : public virtual FullTextException
{
    //IKAnalyzerException (const IKAnalyzerException &);
    IKAnalyzerException & operator = (const IKAnalyzerException&);
    const static ErrorCode_t error_code = ERR_IKANALYZER;
public:
    IKAnalyzerException ();
    explicit IKAnalyzerException (const char* msg);
    virtual ~IKAnalyzerException() throw() {}
};

// fulltext resource operation exception
class FounderXDB_EXPORT FulltextResOperationException : public virtual FullTextException
{
    //FulltextResOperationException (const FulltextResOperationException &);
    FulltextResOperationException & operator = (const FulltextResOperationException&);
    const static ErrorCode_t error_code = ERR_FULLTEXT_RES_OPERATION;
public:
    FulltextResOperationException ();
    explicit FulltextResOperationException (const char* msg);
    virtual ~FulltextResOperationException() throw() {}
};

// use dictionary operation exception ERR_USE_DICTIONARY
class FounderXDB_EXPORT UseDictionaryException : public virtual FullTextException
{
    //UseDictionaryException (const UseDictionaryException &);
    UseDictionaryException & operator = (const UseDictionaryException&);
    const static ErrorCode_t error_code = ERR_USE_DICTIONARY;
public:
    UseDictionaryException ();
    explicit UseDictionaryException (const char* msg);
    virtual ~UseDictionaryException() throw() {}
};

class FounderXDB_EXPORT BadKeyException : public virtual OtherBaseException
{
    BadKeyException & operator = (const BadKeyException&);
    const static ErrorCode_t error_code = ERR_BAD_KEY;
public:
    BadKeyException ();
    explicit BadKeyException (const char* msg);
    virtual ~BadKeyException() throw() {}
};

class FounderXDB_EXPORT OverFlowException : public virtual ExecutionEngineException
{
    OverFlowException & operator = (const OverFlowException&);
    const static ErrorCode_t error_code = ERR_BAD_KEY;
public:
    OverFlowException ();
    explicit OverFlowException (const char* msg);
    virtual ~OverFlowException() throw() {}
};

//the exception for 'ERR_TRUNCATE_INDEX'.
class FounderXDB_EXPORT TruncateException : public virtual ExecutionEngineException
{
    //TruncateException (const TruncateException &);
    TruncateException & operator = (const TruncateException&);
    const static ErrorCode_t error_code  =  ERR_TRUNCATE_INDEX;
public:
    TruncateException ();
    explicit TruncateException (const char* msg);
    virtual  ~TruncateException() throw() {}
};

class FounderXDB_EXPORT KVTableException : public virtual XdbBaseException 
{
public:
    explicit KVTableException (const ErrorCode_t error) ;
    KVTableException (const ErrorCode_t error, const char* msg );
    virtual ~KVTableException() throw() {}
} ;

//the exception for 'ERR_KV_TABLE_EXIST'.
class FounderXDB_EXPORT KVTableExistException : public virtual KVTableException
{
    KVTableExistException & operator = (const KVTableExistException&);
    const static ErrorCode_t error_code  =  ERR_KV_TABLE_EXIST;
public:
    KVTableExistException ();
    explicit KVTableExistException (const char* msg);
    virtual  ~KVTableExistException() throw() {}
};

//the exception for 'ERR_KV_TABLE_NOT_EXIST'.
class FounderXDB_EXPORT KVTableNotExistException : public virtual KVTableException
{
    KVTableNotExistException & operator = (const KVTableNotExistException&);
    const static ErrorCode_t error_code  =  ERR_KV_TABLE_NOT_EXIST;
public:
    KVTableNotExistException ();
    explicit KVTableNotExistException (const char* msg);
    virtual  ~KVTableNotExistException() throw() {}
};

//the exception for 'ERR_KV_TABLE_EXIST'.
class FounderXDB_EXPORT KVIndexTableExistException : public virtual KVTableException
{
    KVIndexTableExistException & operator = (const KVIndexTableExistException&);
    const static ErrorCode_t error_code  =  ERR_KV_INDEX_TABLE_EXIST;
public:
    KVIndexTableExistException ();
    explicit KVIndexTableExistException (const char* msg);
    virtual  ~KVIndexTableExistException() throw() {}
};

//the exception for 'ERR_KV_INDEX_TABLE_NOT_EXIST'.
class FounderXDB_EXPORT KVIndexTableNotExistException : public virtual KVTableException
{
    KVIndexTableNotExistException & operator = (const KVIndexTableNotExistException&);
    const static ErrorCode_t error_code  =  ERR_KV_INDEX_TABLE_NOT_EXIST;
public:
    KVIndexTableNotExistException ();
    explicit KVIndexTableNotExistException (const char* msg);
    virtual  ~KVIndexTableNotExistException() throw() {}
};

class FounderXDB_EXPORT KVTableDumpFailedException : public virtual KVTableException
{
    KVTableDumpFailedException & operator = (const KVTableDumpFailedException&);
    const static ErrorCode_t error_code  =  ERR_KV_TABLE_DUMP_FAILED;
public:
    KVTableDumpFailedException ();
    explicit KVTableDumpFailedException (const char* msg);
    virtual  ~KVTableDumpFailedException() throw() {}
};

class FounderXDB_EXPORT KVTableLoadFailedException : public virtual KVTableException
{
    KVTableLoadFailedException & operator = (const KVTableLoadFailedException&);
    const static ErrorCode_t error_code  =  ERR_KV_TABLE_LOAD_FAILED;
public:
    KVTableLoadFailedException ();
    explicit KVTableLoadFailedException (const char* msg);
    virtual  ~KVTableLoadFailedException() throw() {}
};

class FounderXDB_EXPORT KVIndexIsUniqueException : public virtual KVTableException
{
	KVIndexIsUniqueException & operator = (const KVIndexIsUniqueException&);
	const static ErrorCode_t error_code  =  ERR_KV_TABLE_INDEX_UNIQUE;
public:
	KVIndexIsUniqueException ();
	explicit KVIndexIsUniqueException (const char* msg);
	virtual  ~KVIndexIsUniqueException() throw() {}
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Base exception for all other exceptions which need rollback transaction;
// Virtual inherit form this class.
class FounderXDB_EXPORT ServerAbortTxnException 
{
private:
    // ERR_BASE_POINT_ROLLBACK start, all this for the 'ServerAbortTxnException'
    static const int32 ERR_BASE_POINT_ROLLBACK = -20000; 
    static const int32 ERR_BASE_POINT_LIMIT = -40000;
    const static uint32 EXCEPTION_INFO_BUFLEN = 512*2;
    ErrorCode_t m_errorCode;
    // Don't use std::string here, otherwise it's likely to ////throw another exception 
    // while handling the exception.
    char m_infobuf[EXCEPTION_INFO_BUFLEN+1];
protected:
    
    const ServerAbortTxnException& operator= (const ServerAbortTxnException& obj) ;
public:
    const static int32 ERR_SERVER_ABORT_TXN = ERR_BASE_POINT_ROLLBACK + -1;
    const static int32 ERR_DUPLICATE_MODULE = ERR_BASE_POINT_ROLLBACK + -2;
    const static int32 ERR_UPDATE_CONCURRENTLY = ERR_BASE_POINT_ROLLBACK + -3;
    const static int32 ERR_DEADLOCK_DETECTED=ERR_BASE_POINT_ROLLBACK + -4;
    const static int32 ERR_SERIALIZATION_FAILURE=ERR_BASE_POINT_ROLLBACK + -5;
    
    static int32 getUpperErrorCode () {
        return ServerAbortTxnException::ERR_BASE_POINT_ROLLBACK;
    } 
    static int32 getLowerErrorCode () {
        return ServerAbortTxnException::ERR_BASE_POINT_LIMIT ;
    }
public:
    // Constructor.
    explicit ServerAbortTxnException(const ErrorCode_t errcode) ;
    ServerAbortTxnException(const ErrorCode_t errcode, const char *msg);
    /// Destructor
    virtual ~ServerAbortTxnException() throw() {}
    static const char* GetErrorMessage (const ErrorCode_t error) ;

    void append_info(const char *info)
    {
          //strncat (m_infobuf, "|", strlen ("|"));
          //strncat (m_infobuf, info, strlen(info));      
          uint32 left_chars = EXCEPTION_INFO_BUFLEN - (uint32)strlen (m_infobuf) ;
          if (left_chars > 1)     
              snprintf (m_infobuf+strlen (m_infobuf), left_chars-1, "|%s", info) ;
    };
    inline ErrorCode_t getErrorCode() const { return m_errorCode; }  
    const char* what () { return m_infobuf; };        
    static void ThrowExceptionsByCode (const ErrorCode_t error) ;
    static void ThrowExceptionsByCode (const ErrorCode_t error, const char* msg);
protected:
    virtual void Init (const ErrorCode_t error) ;
    virtual void Init (const ErrorCode_t error, const char *msg );
};

// the ServerAbortTxnException begin
class FounderXDB_EXPORT DuplicateModuleException: public virtual ServerAbortTxnException 
{
    //DuplicateModuleException (const DuplicateModuleException &);
    DuplicateModuleException & operator = (const DuplicateModuleException&);
    const static ErrorCode_t error_code = ERR_DUPLICATE_MODULE;
public:
    DuplicateModuleException ();
    explicit DuplicateModuleException (const char* msg);
    virtual ~DuplicateModuleException() throw() {}
};

class FounderXDB_EXPORT UpdateConcurrentlyException: public virtual ServerAbortTxnException
{
    UpdateConcurrentlyException & operator = (const UpdateConcurrentlyException&);
    const static ErrorCode_t error_code = ERR_UPDATE_CONCURRENTLY;
public:
    UpdateConcurrentlyException ();
    explicit UpdateConcurrentlyException (const char* msg);
    virtual ~UpdateConcurrentlyException() throw() {}
};

class FounderXDB_EXPORT DeadLockException: public virtual ServerAbortTxnException
{
	DeadLockException & operator = (const DeadLockException&);
	const static ErrorCode_t error_code = ERR_DEADLOCK_DETECTED;
public:
	DeadLockException ();
	explicit DeadLockException (const char* msg);
	virtual ~DeadLockException() throw() {}
};

class FounderXDB_EXPORT GraphEngineException : public virtual XdbBaseException 
{
protected:
	GraphEngineException & operator = (const GraphEngineException&);
public:
    explicit GraphEngineException (const ErrorCode_t error) ;
    GraphEngineException (const ErrorCode_t error, const char* msg );
    virtual ~GraphEngineException() throw() {}
} ;

class FounderXDB_EXPORT GraphOutOfMemoryException : public virtual GraphEngineException
{
    GraphOutOfMemoryException & operator = (const GraphOutOfMemoryException&);
	const static ErrorCode_t error_code = ERR_GRAPH_OUTOFMEMORY;
public:
    GraphOutOfMemoryException ();
    explicit GraphOutOfMemoryException (const char* msg);
    virtual  ~GraphOutOfMemoryException() throw() {}
};
class FounderXDB_EXPORT GraphDropException : public virtual GraphEngineException
{
	GraphDropException & operator = (const GraphDropException&);
	const static ErrorCode_t error_code = ERR_GRAPH_DROP;
public:
	GraphDropException ();
	explicit GraphDropException (const char* msg);
	virtual  ~GraphDropException() throw() {}
};

class FounderXDB_EXPORT GraphModelNotExistException : public virtual GraphEngineException
{
    GraphModelNotExistException & operator = (const GraphModelNotExistException&);
	const static ErrorCode_t error_code = ERR_GRAPH_GRAPH_MODEL_NOT_EXIST;
public:
    GraphModelNotExistException ();
    explicit GraphModelNotExistException (const char* msg);
    virtual  ~GraphModelNotExistException() throw() {}
};

class FounderXDB_EXPORT GraphCurrentGraphNotExistException : public virtual GraphEngineException
{
    GraphCurrentGraphNotExistException & operator = (const GraphCurrentGraphNotExistException&);
	const static ErrorCode_t error_code = ERR_GRAPH_CURRENT_GRAPH_NOT_EXIST;
public:
    GraphCurrentGraphNotExistException ();
    explicit GraphCurrentGraphNotExistException (const char* msg);
    virtual  ~GraphCurrentGraphNotExistException() throw() {}
};
class FounderXDB_EXPORT GraphAddTripleException : public virtual GraphEngineException
{
	GraphAddTripleException & operator = (const GraphAddTripleException&);
	const static ErrorCode_t error_code = ERR_GRAPH_ADD_TRIPLE;
public:
	GraphAddTripleException ();
	explicit GraphAddTripleException (const char* msg);
	virtual  ~GraphAddTripleException() throw() {}
};
class FounderXDB_EXPORT GraphDeleteTripleException : public virtual GraphEngineException
{
	GraphDeleteTripleException & operator = (const GraphDeleteTripleException&);
	const static ErrorCode_t error_code = ERR_GRAPH_DELETE_TRIPLE;
public:
	GraphDeleteTripleException ();
	explicit GraphDeleteTripleException (const char* msg);
	virtual  ~GraphDeleteTripleException() throw() {}
};
class FounderXDB_EXPORT GraphUpdateTripleException : public virtual GraphEngineException
{
	GraphUpdateTripleException & operator = (const GraphUpdateTripleException&);
	const static ErrorCode_t error_code = ERR_GRAPH_UPDATE_TRIPLE;
public:
	GraphUpdateTripleException ();
	explicit GraphUpdateTripleException (const char* msg);
	virtual  ~GraphUpdateTripleException() throw() {}
};
class FounderXDB_EXPORT GraphSearchTripleException : public virtual GraphEngineException
{
	GraphSearchTripleException & operator = (const GraphSearchTripleException&);
	const static ErrorCode_t error_code = ERR_GRAPH_SEARCH_TRIPLE;
public:
	GraphSearchTripleException ();
	explicit GraphSearchTripleException (const char* msg);
	virtual  ~GraphSearchTripleException() throw() {}
};

class FounderXDB_EXPORT GraphParseFileNotExistException : public virtual GraphEngineException
{
	GraphParseFileNotExistException & operator = (const GraphParseFileNotExistException&);
	const static ErrorCode_t error_code = ERR_GRAPH_PARSE_FILE_NOT_EXIST;
public:
	GraphParseFileNotExistException ();
	explicit GraphParseFileNotExistException (const char* msg);
	virtual  ~GraphParseFileNotExistException() throw() {}
};

class FounderXDB_EXPORT GraphResultTypeException : public virtual GraphEngineException
{
	GraphResultTypeException & operator = (const GraphResultTypeException&);
	const static ErrorCode_t error_code = ERR_GRAPH_RESULT_TYPE;
public:
	GraphResultTypeException ();
	explicit GraphResultTypeException (const char* msg);
	virtual  ~GraphResultTypeException() throw() {}
};

class FounderXDB_EXPORT GraphUniversalException : public virtual GraphEngineException
{
protected:
	GraphUniversalException & operator = (const GraphUniversalException&);
	const static ErrorCode_t error_code = ERR_GRAPH_UNIVERSAL_POINT;
public:
    GraphUniversalException () ;
    GraphUniversalException (const char* msg );
    virtual ~GraphUniversalException() throw() {}

};

class FounderXDB_EXPORT QueryInterruptedException : public virtual QueryEngineException
{
protected:
	QueryInterruptedException & operator = (const QueryInterruptedException&);
	const static ErrorCode_t error_code = ERR_QUERYENGINE_INTERRUPTED;
public:
	QueryInterruptedException () ;
	QueryInterruptedException (const char* msg );
	virtual ~QueryInterruptedException() throw() {}

};

class FounderXDB_EXPORT QueryTimeoutException : public virtual QueryEngineException
{
protected:
	QueryTimeoutException & operator = (const QueryTimeoutException&);
	const static ErrorCode_t error_code = ERR_QUERYENGINE_TIMEOUT;
public:
	QueryTimeoutException () ;
	QueryTimeoutException (const char* msg );
	virtual ~QueryTimeoutException() throw() {}

};


}  // namespace EXCEPTION
}  // namespace FounderXDB
#endif // !_XDB_EXCEPTIONS_H_
