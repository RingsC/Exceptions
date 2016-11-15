#include "exception/XdbExceptions.h"
#include <sstream>

namespace FounderXDB {
namespace EXCEPTION {

typedef int ErrorCode_t;

void ExceptionsThrow::ThrowExceptions (ErrorCode_t errorcode)
{
    if ((errorcode>XdbBaseException::getLowerErrorCode()) && (errorcode <XdbBaseException::getUpperErrorCode())) {
        XdbBaseException::ThrowExceptionsByCode (errorcode) ;
    } else  if (errorcode> ServerAbortTxnException::getLowerErrorCode() && (errorcode < ServerAbortTxnException::getUpperErrorCode())) {
        ServerAbortTxnException::ThrowExceptionsByCode (errorcode) ;
    }
    return ;
}
void ExceptionsThrow::ThrowExceptions (ErrorCode_t errorcode, const char* msg) 
{
    if ((errorcode>XdbBaseException::getLowerErrorCode()) && (errorcode <XdbBaseException::getUpperErrorCode())) {
        XdbBaseException::ThrowExceptionsByCode (errorcode, msg) ;
    } else  if (errorcode> ServerAbortTxnException::getLowerErrorCode() && (errorcode < ServerAbortTxnException::getUpperErrorCode())) {
        ServerAbortTxnException::ThrowExceptionsByCode (errorcode, msg) ;
	} else {
		XdbBaseException::ThrowExceptionsByCode (XdbBaseException::ERR_UNKNOWN_EXCEPTION, "") ;
	}
    return ;
}

XdbBaseException::XdbBaseException(const ErrorCode_t errcode)
{
     m_errorCode = errcode; memset(m_infobuf, 0, sizeof(m_infobuf)); 
}
XdbBaseException::XdbBaseException(const XdbBaseException& obj)
{
    m_errorCode = obj.getErrorCode();
    memcpy(m_infobuf, obj.m_infobuf, sizeof(m_infobuf));
}
const XdbBaseException& XdbBaseException::operator= (const XdbBaseException& obj)
{
    m_errorCode = obj.getErrorCode();
    memcpy(m_infobuf, obj.m_infobuf, sizeof(m_infobuf));
    return obj;
}
XdbBaseException::XdbBaseException (const ErrorCode_t errcode, const char *msg) 
{     
    m_errorCode = errcode; strncpy(m_infobuf, msg, EXCEPTION_INFO_BUFLEN); 
}

///THE STATIC FUNCTION, USED FOR GETTING THE CORRESPONDING ERROR MESSAGE.
const char* XdbBaseException::GetErrorMessage (const ErrorCode_t error) 
{
    if (ERR_BASE_POINT_STORAGENGINE < error && error <= ERR_BASE_POINT_NET) { //in net error scope.
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_NET) ;
        const char* err_ptr = *(Error_Message_Net + temp_code) ;
        return (XDBSTR(err_ptr));
    }else if ( ERR_BASE_POINT_QUERYENGINE < error && error <= ERR_BASE_POINT_STORAGENGINE ){ // storage engine.
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_STORAGENGINE) ;
        const char* err_ptr = *(Error_Message_StorageEngine + temp_code) ;
        return (XDBSTR(err_ptr));
    }else if (ERR_BASE_POINT_DB < error && error <= ERR_BASE_POINT_QUERYENGINE) {   //query engine
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_QUERYENGINE) ;
        const char* err_ptr = *(Error_Message_QueryEngine + temp_code) ;
        return (XDBSTR(err_ptr));
    } else if (ERR_BASE_POINT_OTHER < error && error <= ERR_BASE_POINT_DB) { //db error 
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_DB) ;
        const char* err_ptr = *(Error_Message_DB + temp_code) ;
        return (XDBSTR(err_ptr));
    } else if (ERR_EXECUTION_ENGINE_BASE < error && error <= ERR_BASE_POINT_OTHER) { //other error info.
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_OTHER) ;
        const char* err_ptr = *(Error_Message_Other + temp_code) ;
        return (XDBSTR(err_ptr));
    } else if (ERR_BASE_POINT_FULLTEXT < error && error <= ERR_EXECUTION_ENGINE_BASE) { //execution engine error info.
        ErrorCode_t temp_code = abs(error - ERR_EXECUTION_ENGINE_BASE) ;
        const char* err_ptr = *(Error_Message_Array_Execution + temp_code) ;
        return (XDBSTR(err_ptr));
    } else if ( ERR_BASE_POINT_KVTABLE < error && error<= ERR_BASE_POINT_FULLTEXT ) { //fulltext error message array. 
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_FULLTEXT) ;
        const char* err_ptr = *(Error_Message_Array_FullText + temp_code) ;
        //const char* err_str = XDBSTR(err_ptr);
        //return (err_str);
        return (XDBSTR(err_ptr));
    } else if ( ERR_BASE_POINT_ROLLBACK < error && error<= ERR_BASE_POINT_KVTABLE ) { //kvtable error message array. 
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_KVTABLE) ;
        const char* err_ptr = *(Error_Message_KVTable + temp_code) ;
        return (XDBSTR(err_ptr));
    }else if ( ERR_BASE_POINT_GRAPHENGINE < error && error<= ERR_BASE_POINT_ROLLBACK ) { //RollBack Transaction error message array. 
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_ROLLBACK) ;
        const char* err_ptr = *(Error_Message_RollBack + temp_code) ;
        return (XDBSTR(err_ptr));
    }else if ( ERR_BASE_POINT_LIMIT < error && error<= ERR_BASE_POINT_GRAPHENGINE){  //GraphEngine error message array. 
    	ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_GRAPHENGINE) ;
        const char* err_ptr = *(Error_Message_GraphEngine + temp_code) ;
        return (XDBSTR(err_ptr));
    }else {
        return XDBSTR ("Unrecognized error information...\n"); 
    }
} 

void XdbBaseException::ThrowExceptionsByCode (const ErrorCode_t error) 
{//a disgusting code here, will be optimized with a new mapping mechanism in future.
        switch (error){  
            case XdbBaseException::ERR_DB_INVALIDE_USER: {   //invalid db user exception.
                 throw InvalidUserException();
            } break;          
            case XdbBaseException::ERR_DB_INVALIDE_PASSWORD : {  //the invalid db password exception.
                 throw InvalidPasswordException ();
            } break; 
            case XdbBaseException::ERR_DB_INVALIDE_STATEMENT: {  //the invalid statement exception.
                 throw InvalidQueryStatementException  ();
            } break;  
            case XdbBaseException::ERR_DB_USER_EXISTS : {        //the db user exists exception.
                 throw UserHasExistException(); 
            } break; 
            case XdbBaseException::ERR_DB_USER_NOT_EXISTS: {     //the db user does not exist exception. 
                 throw UserNotExistsException ();
            } break; 
			case XdbBaseException::ERR_STORAGE_OBJECT_NOT_EXIST: {     //the tablespace does not exist exception. 
				throw TablespaceNotExistsException ();
		    } break; 
            case XdbBaseException::ERR_STORAGE_TABLESPACE_REMOVE: {    
                throw TablespaceRemoveException();
            } break; 
			case XdbBaseException::ERR_STORAGE_EXECUTE_BACKUP: {
				throw ExecuteBackupException();
			}break;
			case XdbBaseException::ERR_STORAGE_EXECUTE_RESTORE: {
				throw ExecuteRestoreException();
			}break;
			case XdbBaseException::ERR_STORAGE_SYSTEM_STATUS: {
				throw CheckSystemStatusException();
			}break;
            case XdbBaseException::ERR_DB_ROLE_EXISTS : {        //the db role exists exception.
                 throw RoleHasExistException (); 
            } break;
            case XdbBaseException::ERR_DB_ROLE_NOT_EXISTS : {    //db role does not exist exception.
                 throw RoleNotExistsException ();
            } break; 
            case XdbBaseException::ERR_DB_NOT_AUTHORIZED: {      //db not authorized exception.
                 throw NotAuthorizedException () ;
            } break; 
            case XdbBaseException::ERR_DATABASE_NOT_USED : {
                 throw DatabaseNotUsedException () ;
            } break;
            case XdbBaseException::ERR_NET_INVALIDE_IP : {       //the invalid ip address exception.
                 throw InvalidIPAddrException ();
            } break; 
            case XdbBaseException::ERR_NET_SSL : {       //the ssl exception.
                throw SslException ();
            } break; 
            case XdbBaseException::ERR_NET_SERVER_NOT_OPEN : {   //the server not open exception.
                 throw ServerNotOpenException ();
            } break; 
            case XdbBaseException::ERR_NET_INVALID_CERTIFICATES : { //the invalid certificates exception.
                 throw InvalidCertificateException ();
            } break; 
            case XdbBaseException::ERR_NET_UNKNOWNCMMD : {        //the unkown command exception.
                 throw UnkownCommandException();
            } break;
            case XdbBaseException::ERR_NET_CONNECTION_CLOSED:{     //the connection closed exception.
                 throw ConnectionClosedException();
            } break;
            case XdbBaseException::ERR_NET_NO_TRANSACTION:{        //no transaction exists exception.
                 throw NoTransactionException();
            } break;
            case XdbBaseException::ERR_NET_TRANSACTION_EXIST:{        //transaction also exists exception.
                throw TransactionExistException();
            } break;
            case XdbBaseException::ERR_NET_DATANODE_DOWN:{        //data node down exception.
                throw DataNodeDownException();
            } break;
			case XdbBaseException::ERR_NET_CLUSTER_NOT_READY:{
				throw ClusterEnvironmentException();
			}break;
            case XdbBaseException::ERR_NET_EXECUTOR_REQUEST_CREATE_FAILED:{   //executor creation failed exception.
                 throw ExecutorCreateException();
            } break;
            case XdbBaseException::ERR_NET_NOT_LOGIN:{              //not login exception.      
                 throw NotLoginExeception();
            } break;
            case XdbBaseException::ERR_NET_RECURSIVE_LOGIN:{              //not recursive login exception.      
                throw NotRecursiveLogin();
            } break;            
            case XdbBaseException::ERR_SOCKET_EXCEPTION:{           //socket exception.
                 throw SocketException();
            } break;
            case XdbBaseException::ERR_COMMAND_SYNTAX:{           //param number err exception.
                 throw CommandSyntaxException();
            } break;
            case XdbBaseException::ERR_VIOLATE_SEGMENTATION_FAULT: {//violate segmenation fault exception.  
                 throw ViolateSegmentationException() ;
            } break; 
            case XdbBaseException::ERR_VIOLATE_OUT_OF_BOUNDARY :{   //out of boundary exception. 
                 throw OutOfBoundaryException ();
            } break;
            case XdbBaseException::ERR_UNKNOWN_COMMAND : {          //unknown command exception.
            } break; 
            case XdbBaseException::ERR_NOT_SUPPORTED_FEATURE : {    //not supported feature exception.
                 throw NotSupportedException();
            } break; 
            case XdbBaseException::ERR_OBJECT_NOT_DESTROYED : {     //object does not destory exception.
                 throw ObjectNotDestroyedException (0);
            } break; 
            case XdbBaseException::ERR_OUT_OF_MEMORY : {           //out of memory exception.
                 throw OutOfMemoryException ();
            } break; 
            case XdbBaseException::ERR_NULL_POINTER: {             //null pointer exception.
                 throw NullPointerException(); 
            } break;
            case XdbBaseException::ERR_EMPTY_STRING: {             //
                 throw EmptyStringException(); 
            } break;
			case XdbBaseException::ERR_FILE_OPEN_FAILED: {         // file open failed 
				 throw OpenFileFailedException(); 
			} break;
            case XdbBaseException::ERR_DOC_CONTENT: {         //
                throw DocContentException(); 
            } break;
			case XdbBaseException::ERR_FILE_EMPTY: {             // empty file
				 throw FileEmptyException(); 
			} break;  
            /*ExecutionEngine Exception Start*/
            case XdbBaseException::ERR_OPEN_CONTAINER : {
                 throw OpenContainerException();
            } break;
            case XdbBaseException::ERR_NOTEXISTS_CONTAINER : {
                 throw ContainerNotExistException();
            } break;
            case XdbBaseException::ERR_RE_INDEX : {
                throw ReIndexException();
            } break;
            case XdbBaseException::ERR_GRANT_NODE : {
                throw GrantNodeErroException();
            } break;
            case XdbBaseException::ERR_CHECK_NODE_PRIV : {
                throw CheckNodePrivErroException();
            } break;
            case XdbBaseException::ERR_LIST_PRIV_NODE : {
                throw ListNodePrivErroException();
            } break;
            case XdbBaseException::ERR_REVOKE_NODE : {
                throw RevokeNodeErroException();
            } break;
            case XdbBaseException::ERR_REMOVE_CONTAINER  : {
                 throw RemoveContainerException ();
            } break;
            case XdbBaseException::ERR_CREATE_CONTAINER : {
                 throw CreateContainerException();
            } break;
            case XdbBaseException::ERR_DUMP_CONTAINER : {
                throw DumpContainerException();
            } break;
            case XdbBaseException::ERR_LOAD_CONTAINER : {
                throw LoadContainerException();
            } break;
            case XdbBaseException::ERR_QUERY : {
                 throw QueryStatementException();
            } break; 
            case XdbBaseException::ERR_ADD_DOCUMENT : {
                 throw AddDocumentException();
            } break;
            case XdbBaseException::ERR_CONTAINER_ALREADY_EXISTS : {
                 throw ContainerAlreadyExistException ();
            } break; 
            case XdbBaseException::ERR_CONTAINER_FULL : {
                throw ContainerFullException ();
            } break; 
            case XdbBaseException::ERR_UPDATE_DOCUMENT:{
                 throw UpdateDocumentsException ();
            } break; 
            case XdbBaseException::ERR_REMOVE_DOCUMENT : {
                 throw RemoveDocumentException();
            } break; 
            case XdbBaseException::ERR_RENAME_CONTAINER : {
                throw RenameContainerException();
            } break;
            case XdbBaseException::ERR_CANNOT_OPEN_CONTAINER : {
                 throw CanNotOpenContainerException();
            } break;
            case XdbBaseException::ERR_ADD_INDEX : {
                 throw AddIndexException();
            } break;
            case XdbBaseException::ERR_TRUNCATE_INDEX : {
                 throw TruncateException();
            } break;
            case XdbBaseException::ERR_REMOVE_INDEX : {
                 throw RemoveIndexException();
            } break;
            case XdbBaseException::ERR_NO_PATHINDEX : {
                 throw NoPathIndexException();
            } break;
            case XdbBaseException::ERR_PATHINDEX_FAIL : {
                 throw PathIndexFailException();
            } break;
            case XdbBaseException::ERR_PATHINDEX_REPEAT : {
                throw PathIndexRepeatException();
            } break;
            case XdbBaseException::ERR_HAVE_PATHINDEX : {
                 throw HavePathIndexException();
            } break;
            case XdbBaseException::ERR_PS_EXECUTE : {
                 throw PSExecuteException();
            } break;
            case XdbBaseException::ERR_PARAMETER_INVALID : {
                 throw ParameterInvalidException();
            } break;
            case XdbBaseException::ERR_CAN_NOT_OPEN_SYSTEM_DB : {
                 throw CanNotOpenSystemDBException();
            } break;
            case XdbBaseException::ERR_CAN_NOT_ACCESS_SYSTEM_DB : {
                 throw CanNotAccessSystemDBException();
            } break;
            case XdbBaseException::ERR_DATABASE_ALREADY_EXIST : {
                 throw DatabaseAlreadyExistException();
            } break;
            case XdbBaseException::ERR_OBJECT_ALREADY_EXIST : {
                throw ObjectAlreadyExistException();
            } break;
            case XdbBaseException::ERR_OBJECT_NOT_EXIST : {
                throw ObjectNotExistException();
            } break;
            case XdbBaseException::ERR_USER_ALREADY_EXIST : {
                 throw UserAlreadyExistException();
            } break;
            case XdbBaseException::ERR_USER_NOT_EXIST : {
                 throw UserNotExistException();
            } break;
            case XdbBaseException::ERR_PASSWORD_INCORRECT : {
                 throw PasswordIncorrectException();
            } break;
            case XdbBaseException::ERR_USEROLE_IS_NOT_ADMIN: {
                throw UserRoleIsNotAdminException();
            } break;
            case XdbBaseException::ERR_SEQUENCE_ERROR: {
                throw SequenceErroException();
            } break;
			case XdbBaseException::ERR_NEW_PASSWORD_SAME_WITH_OLD_PASSWORD : {
				throw NewPasswdSameWithOldPasswdException();
	        } break;
            case XdbBaseException::ERR_PREPARED_STATEMENT_NOT_EXIST : {
                 throw PreparedStatementNotExist();
            } break;
            //
            case XdbBaseException::ERROR_ROLE_ALREADY_EXIST : {
                 throw RoleAlreadyExistException();
            } break;
            case XdbBaseException::ERROR_ROLE_NOT_EXIST : {
                 throw RoleNotExistException();
            } break;
            //
            case XdbBaseException::ERROR_CAN_NOT_BIND_USER_NAME : {
                 throw CanNotBindUserNameException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER : {
                 throw CanNotDeleteSystemTableUserException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_USER_ID : {
                 throw CanNotBindUserIDException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER_PRIVILEGE : {
                 throw CanNotDeleteSystemTableUserPrivilegeException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_USER_ID_FROM_ROLE_USER : {
                 throw CanNotBindUserIDFromRoleUserException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_USER_ID : {
                 throw CanNotDeleteSystemTableRoleUserByUserIDException();
            } break;
            //
            case XdbBaseException::ERROR_CAN_NOT_BIND_ROLE_NAME : {
                 throw CanNotBindRoleNameException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE : {
                 throw CanNotDeleteSystemTableRoleException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_ROLE_ID : {
                 throw CanNotBindRoleIDException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE_PRIVILEGE : {
                 throw CanNotDeleteSystemTableRolePrivilegeException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_ROLE_ID_FROM_ROLE_USER : {
                 throw CanNotBindRoleIDFromRoleUserException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_ROLE_ID : {
                 throw CanNotDeleteSystemTableRoleUserByRoleIDException();
            } break;
            case ERR_CAN_NOT_ACCESS_COLLECTION : {  // can not access the collection exception.
                 throw CanNotAccessCollectionException ();
            } break; 
            case ERR_CAN_NOT_ACCESS_DOCFILE: {      //can not access the doc file exception.
                 throw CanNotAccessDocFileException () ;
            } break; 
            case ERROR_COLLECTION_ALREADY_EXIST: {   //the collection already exists exception.
                throw CollectionAlreadyExistException () ;
            }  break; 
            case ERROR_COLLECTION_NOT_EXIST:{        //the collection already does not exist exception.
                throw CollectionNotExistException () ;
            } break; 
            case ERROR_DOCFILE_ALREADY_EXIST: {      //the doc file already exists exception.
                throw DocFileAlreadyExistException () ;
            } break; 
            case ERROR_DOCFILE_NOT_EXIST: {          //the doc file does not exist exception. 
                throw DocFileNotExistException () ;
            }break;
            case ERROR_CAN_NOT_DELETE_SYSTEM_TABLE:{ //can not delete syste table exception. 
                throw CanNotDeleteSystemTableException () ;
            }break;
            case ERROR_CAN_NOT_INSERT_SYSTEM_TABLE: {// can not insert system table exception  
                throw CanNotInsertSystemTableException () ;
            } break;
            case ERROR_CAN_NOT_DELETE_SYSTEM_TREE_TABLE : { //can not delete system tree table exception.
                 throw CanNotDeleteSystemTreeTableException () ;
            } break; 
            case ERROR_CAN_NOT_INSERT_SYSTEM_TREE_TABLE: { //can not insert system tree table exception.
                 throw CanNotInsertSystemTreeTableException();
            }break; 
            case ERROR_RELATIONSHIP_NOT_EXIST:{            //relationship does not exist exception.
                 throw RelationShipNotExistException () ;
            } break; 
            case XdbBaseException::ERROR_USER_OR_ROLE_NOT_EXIST : {
                throw UserOrRoleNotExistException();
            } break; 
            case XdbBaseException::ERROR_OBJECT_PRIVILEGE_NOT_GRANTED : {
                throw ObjPrivsNotGrantedException();
            } break;
            case XdbBaseException::ERROR_SYS_PRIVILEGE_NOT_GRANTED : {
                throw SysPrivsNotGrantedException();
            } break;
            case XdbBaseException::ERROR_DELETE_COLLECTION: {
                throw DeleteCollectionException();
             } break;
            case XdbBaseException::ERROR_PRIVILEGE_NOT_EXIST : {
                throw PrivNotExistException();
            } break;
            case XdbBaseException::ERROR_USER_NOT_ADMIN : {
                throw UserNotAdminException();
            } break;
            case XdbBaseException::ERROR_PATH_NOT_EXIST : {
                throw PathNotExistException();
            } break;
            case XdbBaseException::ERROR_PATH_ALREADY_EXIST : {
                throw PathAlreadyExistException();
            } break;
            case XdbBaseException::ERROR_GET_CURRENT_DATABASE : {
                throw GetCurrentDatabaseException();
            } break;
            case XdbBaseException::ERROR_GET_CURRENT_USER : {
                throw GetCurrentUserException();
            } break;
            case XdbBaseException::ERROR_CHECK_ITEM_RELATION : {
                throw CheckItemRelationException();
            } break;
            case XdbBaseException::ERROR_DOCITEM_RELATION_HAS_EXIST : {
                throw RelationShipHasExistException();
            } break;
            case XdbBaseException::ERR_RESOURCE_NOT_FOUND : {
                throw ResourceNotFoundException();
            } break;
            case XdbBaseException::ERR_CONVERSION_FAILED : {
                throw ConversionFailedException () ;
            } break; 
            case XdbBaseException::ERROR_CAN_NOT_CHANGE_ADMIN_USER : {
                throw CanNotChangeAdminUserException();
            } break;
            case XdbBaseException::ERROR_CAN_NOT_CHANGE_SYS_ADMIN_ROLE : {
                throw CanNotChangeSysAdminRoleException();
            } break; 
            case XdbBaseException::ERR_DATABASE_NAMED_SYNTAX : {
                throw DatabaseNamedSyntaxException();
            } break;   
            case XdbBaseException::ERR_DATABASE_USED : {
                throw DatabaseUsedException();
            } break;     
            case XdbBaseException::ERR_INTERNAL_QUERY : {
                throw InternalQueryException () ;
            } break;
            case XdbBaseException::ERR_NAME_LEGAL : {
                throw NameLegalException () ;
            } break;
            case XdbBaseException::ERR_DATABASE_NOT_EXIST : {
                throw DatabaseNotExistException () ;
            } break; 
            case XdbBaseException::ERR_DATABASE_DELETE_FAIL : {
                throw DatabaseDeleteFailException () ;
            } break; 
            case XdbBaseException::ERR_CREATE_TABLE_SPACE : {
                throw CreateTableSpaceException () ;
            } break; 
            case XdbBaseException::ERR_TABLE_SPACE_ALREADYEXIST : {
                throw TableSpaceAlreadyExistException () ;
            } break;
            case XdbBaseException::ERR_TABLESPACE_NOT_EXIST : {
                throw TableSpaceNotExistException ();
            } break; 
            case XdbBaseException::ERR_CONTAINER_NOT_COMMIT : {
                throw ContainerNotCommitException ();
            } break; 
            case XdbBaseException::ERR_ID_OVERFLOW : {
                throw IDOverFlowException ();
            } break; 
            case XdbBaseException::ERR_SLAVE_READ_ONLY : {
                throw SlaveReadOnlyException () ;
            } break;
            case XdbBaseException::ERR_NOTEXISTS_LARGE_OBJECT : {
                throw LargeObjectNotExistException () ;
            } break;
            case XdbBaseException::ERR_LARGE_OBJECT : {
                throw LargeObjectException () ;
            } break;
            case XdbBaseException::ERR_SQL_DATABASE : {
                throw SqlDatabaseException () ;
            } break;
            /*ExecutionEngine Exception End*/
            /*FullText Exception Start*/
            case XdbBaseException::ERR_RESOURCE_NOT_FOUND_FULLTEXT : {
                throw ResourceNotFoundExceptionFulltext ();
            } break;
            case XdbBaseException::ERR_STOPWORD_NOTFOUND : {
                throw StopwordNotFoundException ();
            } break;
            case XdbBaseException::ERR_FTWINDOW_AND_FTDISTANCE : {
                throw FTWindowAndFTDistanceException ();
            } break;
            case XdbBaseException::ERR_QUERY_CONTAINS_FTCONTENT : {
                throw QueryContainsFtcontentException ();
            } break;
            case XdbBaseException::ERR_FTLANGUAGE_OPTIONS : {
                throw FTLanguageOptionsException ();
            } break;
            case XdbBaseException::ERR_FTCASE_OPTION_EXCEPTION : {
                throw FTCaseOptionException ();
            } break;
            case XdbBaseException::ERR_WEIGHT_VALUE : {
                throw WeightValueException ();
            } break;
            case XdbBaseException::ERR_MILD_NOT_SELECTION : {
                throw MildNotSelectionException ();
            } break;
            case XdbBaseException::ERR_FTMATCH_OPTIONS : {
                throw FTMatchOptionsExceptions ();
            } break;
            case XdbBaseException::ERR_WILDCARD_SYNTAX : {
                throw WildcardSyntaxException ();
            } break;
            case XdbBaseException::ERR_IKANALYZER : {
                throw IKAnalyzerException();
            } break;
            case XdbBaseException::ERR_FULLTEXT_RES_OPERATION : {
                throw FulltextResOperationException();
            } break;
            case XdbBaseException::ERR_USE_DICTIONARY : {
                throw UseDictionaryException();
            } break;
            /*FullText Exception End*/
            /*KVTable Exception Begin*/
            case XdbBaseException::ERR_KV_TABLE_EXIST : {
                throw KVTableExistException();
            } break;
            case XdbBaseException::ERR_KV_TABLE_NOT_EXIST : {
                throw KVTableNotExistException();
            } break;
            case XdbBaseException::ERR_KV_INDEX_TABLE_EXIST : {
                throw KVIndexTableExistException();
            } break;
            case XdbBaseException::ERR_KV_INDEX_TABLE_NOT_EXIST : {
                throw KVIndexTableNotExistException();
            } break;
            case XdbBaseException::ERR_KV_TABLE_DUMP_FAILED : {
                throw KVTableDumpFailedException();
            } break;
            case XdbBaseException::ERR_KV_TABLE_LOAD_FAILED : {
                throw KVTableLoadFailedException();
            } break;
			case XdbBaseException::ERR_KV_TABLE_INDEX_UNIQUE : {
				throw KVIndexIsUniqueException();
			} break;
            /*KVTable Exception End*/
			case XdbBaseException::ERR_GRAPH_OUTOFMEMORY : {
				throw GraphOutOfMemoryException();
			} break;
			case XdbBaseException::ERR_GRAPH_GRAPH_MODEL_NOT_EXIST : {
				throw GraphModelNotExistException();
			} break;
			case XdbBaseException::ERR_GRAPH_CURRENT_GRAPH_NOT_EXIST : {
				throw GraphCurrentGraphNotExistException();
			} break;
			case XdbBaseException::ERR_GRAPH_PARSE_FILE_NOT_EXIST : {
				throw GraphParseFileNotExistException();
			} break;
			case XdbBaseException::ERR_GRAPH_ADD_TRIPLE : {
				throw GraphAddTripleException();
			} break;
			case XdbBaseException::ERR_GRAPH_DELETE_TRIPLE : {
				throw GraphDeleteTripleException();
			} break;
			case XdbBaseException::ERR_GRAPH_UPDATE_TRIPLE : {
				throw GraphUpdateTripleException();
			} break;
			case XdbBaseException::ERR_GRAPH_SEARCH_TRIPLE : {
				throw GraphSearchTripleException();
			} break;
			case XdbBaseException::ERR_GRAPH_DROP : {
				throw GraphDropException();
			} break;			
			case XdbBaseException::ERR_GRAPH_RESULT_TYPE : {
				throw GraphResultTypeException();
			} break;
			case XdbBaseException::ERR_GRAPH_UNIVERSAL_POINT: {
				throw GraphUniversalException();
			} break;
            case XdbBaseException::ERR_BAD_KEY : {
                throw BadKeyException();
            } break;
			case XdbBaseException::ERR_NET_SVRCFG_FILE : {
				throw SrvcfgFileException();
			} break;
            default:{
                 throw UnknownException();
            } break;

        }
}
void XdbBaseException::ThrowExceptionsByCode (const ErrorCode_t error, const char* msg) 
{//a disgusting code here, will be optimized with a new mapping mechanism in future.
	    msg = XDBSTR(msg);
        switch (error){
            case XdbBaseException::ERR_DB_INVALIDE_USER: {
                 throw InvalidUserException(msg);
            } break;          
            case XdbBaseException::ERR_DB_INVALIDE_PASSWORD : {
                 throw InvalidPasswordException (msg);
            } break; 
            case XdbBaseException::ERR_DB_INVALIDE_STATEMENT: {
                 throw InvalidQueryStatementException  (msg);
            } break;  
            case XdbBaseException::ERR_DB_USER_EXISTS : {
                 throw UserHasExistException(msg); 
            } break; 
            case XdbBaseException::ERR_DB_USER_NOT_EXISTS: {
                 throw UserNotExistsException (msg);
            } break; 
			case XdbBaseException::ERR_STORAGE_OBJECT_NOT_EXIST: {     //the tablespace does not exist exception. 
				throw TablespaceNotExistsException(msg);
			} break; 
            case XdbBaseException::ERR_STORAGE_TABLESPACE_REMOVE: {    
                throw TablespaceRemoveException(msg);
            } break;
			case XdbBaseException::ERR_STORAGE_EXECUTE_BACKUP: {
				throw ExecuteBackupException(msg);
			}break;
			case XdbBaseException::ERR_STORAGE_EXECUTE_RESTORE: {
				throw ExecuteRestoreException(msg);
			}break;
			case XdbBaseException::ERR_STORAGE_SYSTEM_STATUS: {
				throw CheckSystemStatusException(msg);
			}break;
            case XdbBaseException::ERR_DB_ROLE_EXISTS : {
                 throw RoleHasExistException (msg); 
            } break;
            case XdbBaseException::ERR_DB_ROLE_NOT_EXISTS : {
                 throw RoleNotExistsException (msg);
            } break; 
            case XdbBaseException::ERR_DB_NOT_AUTHORIZED: {
                 throw NotAuthorizedException (msg) ;
            } break; 
            case XdbBaseException::ERR_DATABASE_NOT_USED : {
                throw DatabaseNotUsedException (msg) ;
            } break;            
            case XdbBaseException::ERR_NET_INVALIDE_IP : {
                 throw InvalidIPAddrException (msg);
            } break; 
            case XdbBaseException::ERR_NET_SSL : {
                throw SslException (msg);
            } break; 
            case XdbBaseException::ERR_NET_SERVER_NOT_OPEN : {
                 throw ServerNotOpenException (msg);
            } break; 
            case XdbBaseException::ERR_NET_INVALID_CERTIFICATES : {
                 throw InvalidCertificateException (msg);
            } break; 
            case XdbBaseException::ERR_NET_UNKNOWNCMMD : {
                 throw UnkownCommandException(msg);
            } break;
            case XdbBaseException::ERR_NET_CONNECTION_CLOSED:{
                 throw ConnectionClosedException(msg);
            } break;
            case XdbBaseException::ERR_NET_NO_TRANSACTION:{
                 throw NoTransactionException(msg);
            } break;
            case XdbBaseException::ERR_NET_TRANSACTION_EXIST:{
                throw TransactionExistException(msg);
            } break;
            case XdbBaseException::ERR_NET_DATANODE_DOWN:{
                throw DataNodeDownException(msg);
            } break;
			case XdbBaseException::ERR_NET_CLUSTER_NOT_READY:{
				throw ClusterEnvironmentException(msg);
			}break;
            case XdbBaseException::ERR_NET_EXECUTOR_REQUEST_CREATE_FAILED:{
                 throw ExecutorCreateException(msg);
            } break;
            case XdbBaseException::ERR_NET_NOT_LOGIN:{
                 throw NotLoginExeception(msg);
            } break;
            case XdbBaseException::ERR_NET_RECURSIVE_LOGIN:{              //not recursive login exception.      
                throw NotRecursiveLogin(msg);
            } break;              
            case XdbBaseException::ERR_SOCKET_EXCEPTION:{
                 throw SocketException(msg);
            } break;
            case XdbBaseException::ERR_COMMAND_SYNTAX:{
                 throw CommandSyntaxException(msg);
            } break;
            case XdbBaseException::ERR_VIOLATE_SEGMENTATION_FAULT: {
                 throw ViolateSegmentationException(msg) ;
            } break; 
            case XdbBaseException::ERR_VIOLATE_OUT_OF_BOUNDARY :{
                 throw OutOfBoundaryException (msg);
            } break;
            case XdbBaseException::ERR_UNKNOWN_COMMAND : {
            } break; 
            case XdbBaseException::ERR_NOT_SUPPORTED_FEATURE : {
                 throw NotSupportedException(msg);
            } break; 
            case XdbBaseException::ERR_OBJECT_NOT_DESTROYED : {
                 throw ObjectNotDestroyedException (0);
            } break; 
            case XdbBaseException::ERR_OUT_OF_MEMORY : {
                 throw OutOfMemoryException (/*msg*/);
            } break; 
            case XdbBaseException::ERR_NULL_POINTER: {
                 throw NullPointerException(msg); 
            } break;
            case XdbBaseException::ERR_EMPTY_STRING: {
                 throw EmptyStringException(msg); 
            } break;
			case XdbBaseException::ERR_FILE_OPEN_FAILED: {         // file open failed 
				 throw OpenFileFailedException(msg); 
			} break;
            case XdbBaseException::ERR_DOC_CONTENT: {         // file open failed 
                throw DocContentException(msg); 
            } break;
			case XdbBaseException::ERR_FILE_EMPTY: {             // empty file
				 throw FileEmptyException(msg); 
			} break;    
            /*ExecutionEngine Exception Start*/
            case XdbBaseException::ERR_OPEN_CONTAINER : {
                 throw OpenContainerException(msg);
            } break;
            case XdbBaseException::ERR_NOTEXISTS_CONTAINER : {
                 throw ContainerNotExistException(msg);
            } break;
            case XdbBaseException::ERR_RE_INDEX : {
                throw ReIndexException(msg);
            } break;
            case XdbBaseException::ERR_GRANT_NODE : {
                throw GrantNodeErroException(msg);
            } break;
            case XdbBaseException::ERR_CHECK_NODE_PRIV : {
                throw CheckNodePrivErroException(msg);
            } break;
            case XdbBaseException::ERR_LIST_PRIV_NODE : {
                throw ListNodePrivErroException(msg);
            } break;
            case XdbBaseException::ERR_REVOKE_NODE : {
                throw RevokeNodeErroException(msg);
            } break;
            case XdbBaseException::ERR_REMOVE_CONTAINER  : {
                 throw RemoveContainerException (msg);
            } break;
            case XdbBaseException::ERR_CREATE_CONTAINER : {
                 throw CreateContainerException(msg);
            } break;
            case XdbBaseException::ERR_LOAD_CONTAINER : {
                throw LoadContainerException(msg);
            } break;
            case XdbBaseException::ERR_DUMP_CONTAINER : {
                throw DumpContainerException(msg);
            } break;
            case XdbBaseException::ERR_QUERY : {
                 throw QueryStatementException(msg);
            } break; 
            case XdbBaseException::ERR_ADD_DOCUMENT : {
                 throw AddDocumentException(msg);
            } break;
            case XdbBaseException::ERR_CONTAINER_ALREADY_EXISTS : {
                 throw ContainerAlreadyExistException (msg);
            } break; 
            case XdbBaseException::ERR_CONTAINER_FULL : {
                throw ContainerFullException (msg);
            } break;
            case XdbBaseException::ERR_UPDATE_DOCUMENT:{
                 throw UpdateDocumentsException (msg);
            } break; 
            case XdbBaseException::ERR_REMOVE_DOCUMENT : {
                 throw RemoveDocumentException(msg);
            } break; 
            case XdbBaseException::ERR_RENAME_CONTAINER : {
                throw RenameContainerException(msg);
            } break; 
            case XdbBaseException::ERR_CANNOT_OPEN_CONTAINER : {
                 throw CanNotOpenContainerException(msg);
            } break;
            case XdbBaseException::ERR_ADD_INDEX : {
                 throw AddIndexException(msg);
            } break;
            case XdbBaseException::ERR_TRUNCATE_INDEX : {
                throw TruncateException(msg);
            } break;
            case XdbBaseException::ERR_REMOVE_INDEX : {
                 throw RemoveIndexException(msg);
            } break;
            case XdbBaseException::ERR_NO_PATHINDEX : {
                throw NoPathIndexException(msg);
            } break;
            case XdbBaseException::ERR_PATHINDEX_FAIL : {
                throw PathIndexFailException(msg);
            } break;
            case XdbBaseException::ERR_PATHINDEX_REPEAT : {
                throw PathIndexRepeatException(msg);
            } break;
            case XdbBaseException::ERR_HAVE_PATHINDEX : {
                throw HavePathIndexException(msg);
            } break;
            case XdbBaseException::ERR_PS_EXECUTE : {
                 throw PSExecuteException(msg);
            } break;
            case XdbBaseException::ERR_PARAMETER_INVALID : {
                 throw ParameterInvalidException(msg);
            } break;
            case XdbBaseException::ERR_CAN_NOT_OPEN_SYSTEM_DB : {
                 throw CanNotOpenSystemDBException(msg);
            } break;
            case XdbBaseException::ERR_CAN_NOT_ACCESS_SYSTEM_DB : {
                 throw CanNotAccessSystemDBException(msg);
            } break;
            case XdbBaseException::ERR_DATABASE_ALREADY_EXIST : {
                 throw DatabaseAlreadyExistException(msg);
            } break;
            case XdbBaseException::ERR_OBJECT_ALREADY_EXIST : {
                throw ObjectAlreadyExistException(msg);
            } break;
            case XdbBaseException::ERR_OBJECT_NOT_EXIST : {
                throw ObjectNotExistException(msg);
            } break;
            case XdbBaseException::ERR_USER_ALREADY_EXIST : {
                 throw UserAlreadyExistException(msg);
            } break;
            case XdbBaseException::ERR_USER_NOT_EXIST : {
                 throw UserNotExistException(msg);
            } break;
            case XdbBaseException::ERR_PASSWORD_INCORRECT : {
                 throw PasswordIncorrectException(msg);
            } break;
            case XdbBaseException::ERR_USEROLE_IS_NOT_ADMIN: {
                throw UserRoleIsNotAdminException(msg);
            } break;
            case XdbBaseException::ERR_SEQUENCE_ERROR: {
                throw SequenceErroException(msg);
            } break;
			case XdbBaseException::ERR_NEW_PASSWORD_SAME_WITH_OLD_PASSWORD : {
				throw NewPasswdSameWithOldPasswdException(msg);
		    } break;
            case XdbBaseException::ERR_PREPARED_STATEMENT_NOT_EXIST : {
                 throw PreparedStatementNotExist(msg);
            } break;
            //
            case XdbBaseException::ERROR_ROLE_ALREADY_EXIST : {
                 throw RoleAlreadyExistException(msg);
            } break;
            case XdbBaseException::ERROR_ROLE_NOT_EXIST : {
                 throw RoleNotExistException(msg);
            } break;
            //
            case XdbBaseException::ERROR_CAN_NOT_BIND_USER_NAME : {
                 throw CanNotBindUserNameException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER : {
                 throw CanNotDeleteSystemTableUserException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_USER_ID : {
                 throw CanNotBindUserIDException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_USER_PRIVILEGE : {
                 throw CanNotDeleteSystemTableUserPrivilegeException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_USER_ID_FROM_ROLE_USER : {
                 throw CanNotBindUserIDFromRoleUserException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_USER_ID : {
                 throw CanNotDeleteSystemTableRoleUserByUserIDException(msg);
            } break;
            //
            case XdbBaseException::ERROR_CAN_NOT_BIND_ROLE_NAME : {
                 throw CanNotBindRoleNameException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE : {
                 throw CanNotDeleteSystemTableRoleException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_ROLE_ID : {
                 throw CanNotBindRoleIDException(msg);
            } break;
             case XdbBaseException::ERROR_CAN_NOT_DELETE_SYSTEM_TABLE_ROLE_PRIVILEGE : {
                 throw CanNotDeleteSystemTableRolePrivilegeException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_BIND_ROLE_ID_FROM_ROLE_USER : {
                 throw CanNotBindRoleIDFromRoleUserException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_DELETE_TABLE_ROLE_USER_BY_ROLE_ID : {
                 throw CanNotDeleteSystemTableRoleUserByRoleIDException(msg);
            } break;
            case ERR_CAN_NOT_ACCESS_COLLECTION : {  // can not access the collection.
                 throw CanNotAccessCollectionException (msg);
            } break; 
            case ERR_CAN_NOT_ACCESS_DOCFILE: { 
                 throw CanNotAccessDocFileException (msg) ;
            } break; 
            case ERROR_COLLECTION_ALREADY_EXIST: {
                throw CollectionAlreadyExistException (msg) ;
            }  break; 
            case ERROR_COLLECTION_NOT_EXIST:{
                throw CollectionNotExistException (msg) ;
            } break; 
            case ERROR_DOCFILE_ALREADY_EXIST: {
                throw DocFileAlreadyExistException (msg) ;
            } break; 
            case ERROR_DOCFILE_NOT_EXIST: {
                throw DocFileNotExistException (msg) ;
            }break;
            case ERROR_CAN_NOT_DELETE_SYSTEM_TABLE:{
                throw CanNotDeleteSystemTableException (msg) ;
            }break;
            case ERROR_CAN_NOT_INSERT_SYSTEM_TABLE: {
                throw CanNotInsertSystemTableException (msg) ;
            } break;
            case ERROR_CAN_NOT_DELETE_SYSTEM_TREE_TABLE : {
                 throw CanNotDeleteSystemTreeTableException (msg) ;
            } break; 
            case ERROR_CAN_NOT_INSERT_SYSTEM_TREE_TABLE: {
                 throw CanNotInsertSystemTreeTableException(msg);
            }break; 
            case ERROR_RELATIONSHIP_NOT_EXIST:{
                 throw RelationShipNotExistException (msg) ;
            } break; 
            case XdbBaseException::ERROR_USER_OR_ROLE_NOT_EXIST : {
                throw UserOrRoleNotExistException(msg);
            } break; 
            case XdbBaseException::ERROR_OBJECT_PRIVILEGE_NOT_GRANTED : {
                throw ObjPrivsNotGrantedException(msg);
            } break;
            case XdbBaseException::ERROR_SYS_PRIVILEGE_NOT_GRANTED : {
                throw SysPrivsNotGrantedException(msg);
            } break;
            case XdbBaseException::ERROR_DELETE_COLLECTION: {
                throw DeleteCollectionException(msg);
             } break;
            case XdbBaseException::ERROR_PRIVILEGE_NOT_EXIST : {
                throw PrivNotExistException(msg);
            } break;
            case XdbBaseException::ERROR_USER_NOT_ADMIN : {
                throw UserNotAdminException(msg);
            } break;
            case XdbBaseException::ERROR_PATH_NOT_EXIST : {
                throw PathNotExistException(msg);
            } break;
            case XdbBaseException::ERROR_PATH_ALREADY_EXIST : {
                throw PathAlreadyExistException(msg);
            } break;
			case XdbBaseException::ERROR_GET_CURRENT_DATABASE : {
                throw GetCurrentDatabaseException();
            } break;
            case XdbBaseException::ERROR_GET_CURRENT_USER : {
                throw GetCurrentUserException();
            } break;
            case XdbBaseException::ERROR_CHECK_ITEM_RELATION : {
                throw CheckItemRelationException();
            } break;
            case XdbBaseException::ERROR_DOCITEM_RELATION_HAS_EXIST : {
                throw RelationShipHasExistException();
            } break;
            case XdbBaseException::ERR_RESOURCE_NOT_FOUND : {
                throw ResourceNotFoundException();
            } break;
            case XdbBaseException::ERR_CONVERSION_FAILED : {
                throw ConversionFailedException (msg);
            } break;  
            case XdbBaseException::ERROR_CAN_NOT_CHANGE_ADMIN_USER : {
                throw CanNotChangeAdminUserException(msg);
            } break;
            case XdbBaseException::ERROR_CAN_NOT_CHANGE_SYS_ADMIN_ROLE : {
                throw CanNotChangeSysAdminRoleException(msg);
            } break;   
            case XdbBaseException::ERR_DATABASE_NAMED_SYNTAX : {
                throw DatabaseNamedSyntaxException(msg);
            } break;
            case XdbBaseException::ERR_DATABASE_USED : {
                throw DatabaseUsedException(msg);
            } break;
            case XdbBaseException::ERR_DATABASE_NOT_EXIST : {
                throw DatabaseNotExistException(msg);
            } break;
            case XdbBaseException::ERR_DATABASE_DELETE_FAIL : {
                throw DatabaseDeleteFailException (msg) ;
            } break; 
            case XdbBaseException::ERR_INTERNAL_QUERY: {
                throw InternalQueryException (msg) ;
            } break;
            case XdbBaseException::ERR_NAME_LEGAL: {
                throw NameLegalException (msg) ;
            } break;
            case XdbBaseException::ERR_CREATE_TABLE_SPACE: {
                throw CreateTableSpaceException (msg) ;
            } break;
            case XdbBaseException::ERR_TABLE_SPACE_ALREADYEXIST : {
                throw TableSpaceAlreadyExistException (msg) ;
            } break;
            case XdbBaseException::ERR_SLAVE_READ_ONLY : {
                throw SlaveReadOnlyException (msg) ;
            } break;
            case XdbBaseException::ERR_NOTEXISTS_LARGE_OBJECT : {
                throw LargeObjectNotExistException (msg) ;
            } break;
            case XdbBaseException::ERR_LARGE_OBJECT : {
                throw LargeObjectException (msg) ;
            } break;
            case XdbBaseException::ERR_TABLESPACE_NOT_EXIST : {
                throw TableSpaceNotExistException(msg);
            } break;
            case XdbBaseException::ERR_CONTAINER_NOT_COMMIT : {
                throw ContainerNotCommitException (msg);
            } break; 
            case XdbBaseException::ERR_ID_OVERFLOW : {
                throw IDOverFlowException (msg);
            } break; 
            case XdbBaseException::ERR_SQL_DATABASE : {
                throw SqlDatabaseException (msg);
            } break;
            /*ExecutionEngine Exception End*/
            /*FullText Exception End*/
            case XdbBaseException::ERR_RESOURCE_NOT_FOUND_FULLTEXT : {
                throw ResourceNotFoundExceptionFulltext(msg);
            } break;
            case XdbBaseException::ERR_STOPWORD_NOTFOUND : {
                throw StopwordNotFoundException (msg);
            } break;
            case XdbBaseException::ERR_FTWINDOW_AND_FTDISTANCE : {
                throw FTWindowAndFTDistanceException (msg);
            } break;
            case XdbBaseException::ERR_QUERY_CONTAINS_FTCONTENT : {
                throw QueryContainsFtcontentException (msg);
            } break;
            case XdbBaseException::ERR_FTLANGUAGE_OPTIONS : {
                throw FTLanguageOptionsException (msg);
            } break;
            case XdbBaseException::ERR_FTCASE_OPTION_EXCEPTION : {
                throw FTCaseOptionException (msg);
            } break;
            case XdbBaseException::ERR_WEIGHT_VALUE : {
                throw WeightValueException (msg);
            } break;
            case XdbBaseException::ERR_MILD_NOT_SELECTION : {
                throw MildNotSelectionException (msg);
            } break;
            case XdbBaseException::ERR_FTMATCH_OPTIONS : {
                throw FTMatchOptionsExceptions (msg);
            } break;
            case XdbBaseException::ERR_WILDCARD_SYNTAX : {
                throw WildcardSyntaxException (msg);
            } break;
            case XdbBaseException::ERR_IKANALYZER : {
                throw IKAnalyzerException (msg);
            } break;
            case XdbBaseException::ERR_FULLTEXT_RES_OPERATION : {
                throw FulltextResOperationException(msg);
            } break;
            case XdbBaseException::ERR_USE_DICTIONARY : {
                throw UseDictionaryException(msg);
            } break;
            /*FullText Exception End*/
            /*KVTable Exception Begin*/
            case XdbBaseException::ERR_KV_TABLE_EXIST : {
                throw KVTableExistException(msg);
            } break;
            case XdbBaseException::ERR_KV_TABLE_NOT_EXIST : {
                throw KVTableNotExistException(msg);
            } break;
            case XdbBaseException::ERR_KV_INDEX_TABLE_EXIST : {
                throw KVIndexTableExistException(msg);
            } break;
            case XdbBaseException::ERR_KV_INDEX_TABLE_NOT_EXIST : {
                throw KVIndexTableNotExistException(msg);
            } break;
            case XdbBaseException::ERR_KV_TABLE_DUMP_FAILED : {
                throw KVTableDumpFailedException(msg);
            } break;
            case XdbBaseException::ERR_KV_TABLE_LOAD_FAILED : {
                throw KVTableLoadFailedException(msg);
            } break;
			case XdbBaseException::ERR_KV_TABLE_INDEX_UNIQUE : {
				throw KVIndexIsUniqueException(msg);
			} break;
            /*KVTable Exception End*/
            /*RollbackTransaction Exception Start*/
			/*RollbackTransaction Exception End*/
			case XdbBaseException::ERR_GRAPH_OUTOFMEMORY : {
				throw GraphOutOfMemoryException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_GRAPH_MODEL_NOT_EXIST : {
				throw GraphModelNotExistException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_CURRENT_GRAPH_NOT_EXIST : {
				throw GraphCurrentGraphNotExistException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_PARSE_FILE_NOT_EXIST : {
				throw GraphParseFileNotExistException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_ADD_TRIPLE : {
				throw GraphAddTripleException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_DELETE_TRIPLE : {
				throw GraphDeleteTripleException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_UPDATE_TRIPLE : {
				throw GraphUpdateTripleException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_SEARCH_TRIPLE : {
				throw GraphSearchTripleException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_DROP : {
				throw GraphDropException(msg);
			} break;			
			case XdbBaseException::ERR_GRAPH_RESULT_TYPE : {
				throw GraphResultTypeException(msg);
			} break;
			case XdbBaseException::ERR_GRAPH_UNIVERSAL_POINT: {
				throw GraphUniversalException(msg);
			} break;			
            case XdbBaseException::ERR_BAD_KEY : {
                throw BadKeyException (msg);
            } break;
			case XdbBaseException::ERR_NET_SVRCFG_FILE : {
				throw SrvcfgFileException(msg);
			} break;
            default:{
                 std::stringstream ss;
                 std::string strErrorCode = "";
                 std::string strmsg = msg;
                 int32 errorCode = error;
                 ss<<errorCode;
                 ss>>strErrorCode;                 
                 strmsg = strmsg + " ErrorCode : " + strErrorCode;
                 throw UnknownException(strmsg.c_str());
            } break;
            
        }
}
void XdbBaseException::Init (const ErrorCode_t error) 
{
     m_errorCode = error;
     memset (m_infobuf, 0, sizeof(m_infobuf));
     const char* error_info = XdbBaseException::GetErrorMessage (error); 
     uint32 length = ((uint32)strlen (error_info) > EXCEPTION_INFO_BUFLEN)? (EXCEPTION_INFO_BUFLEN -1): (uint32)strlen (error_info) ; 
     strncpy (m_infobuf, error_info, length);
}
void XdbBaseException::Init (const ErrorCode_t error, const char *msg) 
{
     m_errorCode = error;
     memset (m_infobuf, 0, sizeof(m_infobuf));
     const char* error_info = XdbBaseException::GetErrorMessage (error); 
     std::string str(error_info), str_la(msg);
     uint32 left_chars (EXCEPTION_INFO_BUFLEN), length (0);
     if (str_la.find(str) == std::string::npos ) {
         length = ((uint32)strlen (error_info) > EXCEPTION_INFO_BUFLEN )? (EXCEPTION_INFO_BUFLEN -1) : (uint32)strlen (error_info) ;
         strncpy (m_infobuf, error_info, length);
         left_chars = EXCEPTION_INFO_BUFLEN - length ;
     } 
     
     if (left_chars > 1 )
         snprintf (m_infobuf+length, left_chars-1,"|%s",msg);
}

/*************************************************************************************************/
// These following codes describe the second-level base classes. 
// 7 sub-base classes include ServerFrameworkException, DatabaseException, ExecutionEngineException, StorageEngineException, OtherBaseException , 
// QueryEngineException, and FullTextException, which correspond to 'Error_Message_Net', 'Error_Message_DB', 'Error_Message_Array_Execution', 'Error_Message_StorageEngine', 
//  'Error_Message_Other' , 'Error_Message_QueryEngine' and 'Error_Message_Array_FullText',''.
// Using the top base exception class directly are not recommended. Using the specific exception class to indicate which type exception occurs.
//
//
//the base class for array of 'Error_Message_Array_Execution'.
ExecutionEngineException::ExecutionEngineException (const ErrorCode_t errcode) : XdbBaseException(errcode)
{    
    Init (errcode) ;
}
    
ExecutionEngineException::ExecutionEngineException(const ErrorCode_t errcode, const char *msg): XdbBaseException(errcode)
{
    Init (errcode, msg);
}

// Base exception for all other exceptions in fulltext.
// Virtual inherit form this class.
// the base class for array of 'Error_Message_Array_FullText', all about the fulltext exceptions are derived from this class.
FullTextException::FullTextException (const ErrorCode_t errcode) : XdbBaseException(errcode)
{    
    Init (errcode) ;
}

FullTextException::FullTextException(const ErrorCode_t errcode, const char *msg): XdbBaseException(errcode)
{
    Init (errcode, msg);
}

// Base exception for all other exceptions in storage engine subsystem.
// Virtual inherit from this class.
//the base class for array of 'Error_Message_StorageEngine', all about the storage engine exceptions are dervied from this class.
StorageEngineException::StorageEngineException (const ErrorCode_t errcode) : XdbBaseException(errcode)
{
    Init (errcode) ;
}

StorageEngineException::StorageEngineException(const ErrorCode_t errcode, const char *msg) : XdbBaseException(errcode)
{
    Init (errcode, msg);
}

// the base class for array of 'Error_Message_Other', all about the other exceptions are derived from this class. 
OtherBaseException::OtherBaseException (const ErrorCode_t errcode) : XdbBaseException(errcode)
{
    Init (errcode) ;
}
     
OtherBaseException::OtherBaseException (const ErrorCode_t errcode, const char* msg) : XdbBaseException(errcode)
{
    Init (errcode, msg);
}

//the base class for array of 'Error_Message_DB', all about the db execeptions are derived from this classed.
DatabaseException::DatabaseException (const ErrorCode_t errcode) : XdbBaseException (errcode)
{
    Init (errcode);
}
     
DatabaseException::DatabaseException (const ErrorCode_t errcode, const char* msg ) : XdbBaseException (errcode)
{
    Init (errcode, msg);
}
// Base exception for all other exceptions in server framework subsystem.
// Virtual inherit from this class. for the array of 'Error_Message_Net'
ServerFrameworkException::ServerFrameworkException (const ErrorCode_t error) : XdbBaseException (error) 
{
    Init (error) ;
}
    
ServerFrameworkException::ServerFrameworkException (const ErrorCode_t error, const char* msg) : XdbBaseException (error) 
{
    Init (error, msg);
}

//the base class for the array of 'Error_Message_QueryEngine', all about the inquiry exceptions are derived from this class.
QueryEngineException::QueryEngineException (const ErrorCode_t error) : XdbBaseException(error) 
{
    Init (error) ;
} 
      
QueryEngineException::QueryEngineException (const ErrorCode_t error, const char* msg ) : XdbBaseException(error) 
{
    Init (error, msg);
}


OutOfMemoryException::OutOfMemoryException() : XdbBaseException (OutOfMemoryException::error_code) ,OtherBaseException (OutOfMemoryException::error_code)
{
    Init (OutOfMemoryException::error_code) ;
}
OutOfMemoryException::OutOfMemoryException(const size_t /*byets*/) : XdbBaseException (OutOfMemoryException::error_code) , OtherBaseException (OutOfMemoryException::error_code)
{
    Init (OutOfMemoryException::error_code) ;
}
OutOfMemoryException::OutOfMemoryException(const size_t /*byets*/, const char* msg) : XdbBaseException (OutOfMemoryException::error_code) , OtherBaseException (OutOfMemoryException::error_code)
{
    Init (OutOfMemoryException::error_code, msg) ;
}

ObjectNotDestroyedException::ObjectNotDestroyedException(const size_t num_leaked): XdbBaseException (ObjectNotDestroyedException::error_code) , OtherBaseException(ObjectNotDestroyedException::error_code)
{
    memset (m_infobuf, 0, sizeof(m_infobuf));
    const char* error_info = XdbBaseException::GetErrorMessage (error_code); 
    snprintf(m_infobuf, sizeof(m_infobuf), error_info, num_leaked);
}
ObjectNotDestroyedException::ObjectNotDestroyedException(const size_t /*num_leaked*/, const char* msg): XdbBaseException (ObjectNotDestroyedException::error_code) ,OtherBaseException(ObjectNotDestroyedException::error_code)
{
    Init (ObjectNotDestroyedException::error_code, msg) ;
}

NotSupportedException::NotSupportedException(): XdbBaseException(NotSupportedException::error_code), OtherBaseException(NotSupportedException::error_code) 
{ 
    Init (NotSupportedException::error_code) ;
}

NotSupportedException::NotSupportedException(const char* msg): XdbBaseException(NotSupportedException::error_code), OtherBaseException(NotSupportedException::error_code) 
{ 
    Init (NotSupportedException::error_code, msg) ;
}

OutOfBoundaryException::OutOfBoundaryException () : XdbBaseException (OutOfBoundaryException::error_code) , OtherBaseException (OutOfBoundaryException::error_code)
{
    Init (OutOfBoundaryException::error_code) ;
}

OutOfBoundaryException::OutOfBoundaryException (const char* msg) : XdbBaseException (OutOfBoundaryException::error_code) , OtherBaseException (OutOfBoundaryException::error_code)
{
    Init (OutOfBoundaryException::error_code, msg) ;
}

NullPointerException::NullPointerException () : XdbBaseException (NullPointerException::error_code) , OtherBaseException (NullPointerException::error_code)
{
    Init (NullPointerException::error_code) ;
}

NullPointerException::NullPointerException (const char* msg) : XdbBaseException (NullPointerException::error_code) ,OtherBaseException (NullPointerException::error_code)
{
    Init (NullPointerException::error_code, msg) ;
}

EmptyStringException::EmptyStringException () : XdbBaseException (EmptyStringException::error_code) , OtherBaseException (EmptyStringException::error_code)
{
    Init (EmptyStringException::error_code) ;
}

EmptyStringException::EmptyStringException (const char* msg) : XdbBaseException (EmptyStringException::error_code) , OtherBaseException (EmptyStringException::error_code)
{
    Init (EmptyStringException::error_code, msg) ;
}

CommandSyntaxException::CommandSyntaxException () : XdbBaseException (CommandSyntaxException::error_code) , OtherBaseException (CommandSyntaxException::error_code)
{
    Init (CommandSyntaxException::error_code) ;
}

CommandSyntaxException::CommandSyntaxException (const char* msg) : XdbBaseException (CommandSyntaxException::error_code) , OtherBaseException (CommandSyntaxException::error_code)
{
    Init (CommandSyntaxException::error_code,msg) ;
}

UnknownException::UnknownException () : XdbBaseException (UnknownException::error_code) , OtherBaseException (UnknownException::error_code)
{
    Init (UnknownException::error_code) ;
}

UnknownException::UnknownException (const char* msg) : XdbBaseException (UnknownException::error_code) , OtherBaseException (UnknownException::error_code)
{
    Init (UnknownException::error_code, msg) ;
}

//throws this exception, ViolateSegmentationException, when segmentation violation error occurs.
ViolateSegmentationException::ViolateSegmentationException () : XdbBaseException (ViolateSegmentationException::error_code),OtherBaseException (ViolateSegmentationException::error_code)
{
    Init (ViolateSegmentationException::error_code) ;
} 
ViolateSegmentationException::ViolateSegmentationException (const char* msg) : XdbBaseException (ViolateSegmentationException::error_code), OtherBaseException (ViolateSegmentationException::error_code)
{
    Init (ViolateSegmentationException::error_code, msg) ;
} 

InvalidUserException::InvalidUserException() : XdbBaseException(InvalidUserException::error_code) ,DatabaseException(InvalidUserException::error_code)
{
    Init (InvalidUserException::error_code) ;
}    
InvalidUserException::InvalidUserException(const char* msg) : XdbBaseException(InvalidUserException::error_code) ,DatabaseException(InvalidUserException::error_code)
{
    Init (InvalidUserException::error_code, msg) ;
}    

InvalidPasswordException::InvalidPasswordException () : XdbBaseException (InvalidPasswordException::error_code) ,DatabaseException (InvalidPasswordException::error_code)
{
    Init (InvalidPasswordException::error_code) ;
}
InvalidPasswordException::InvalidPasswordException (const char* msg) : XdbBaseException (InvalidPasswordException::error_code) ,DatabaseException (InvalidPasswordException::error_code)
{
    Init (InvalidPasswordException::error_code, msg) ;
}

InvalidQueryStatementException::InvalidQueryStatementException () : XdbBaseException (InvalidQueryStatementException::error_code) ,DatabaseException (InvalidQueryStatementException::error_code)
{
    Init (InvalidQueryStatementException::error_code);
}
InvalidQueryStatementException::InvalidQueryStatementException (const char* msg) : XdbBaseException (InvalidQueryStatementException::error_code) ,DatabaseException (InvalidQueryStatementException::error_code)
{
    Init (InvalidQueryStatementException::error_code, msg);
}

InvalidIPAddrException::InvalidIPAddrException () : XdbBaseException (InvalidIPAddrException::error_code) ,ServerFrameworkException (InvalidIPAddrException::error_code)
{
    Init (InvalidIPAddrException::error_code) ;
}
InvalidIPAddrException::InvalidIPAddrException (const char* msg) : XdbBaseException (InvalidIPAddrException::error_code) ,ServerFrameworkException (InvalidIPAddrException::error_code)
{
    Init (InvalidIPAddrException::error_code, msg);
}
SslException::SslException () : XdbBaseException (SslException::error_code) ,ServerFrameworkException (SslException::error_code)
{
    Init (SslException::error_code) ;
}
SslException::SslException (const char* msg) : XdbBaseException (SslException::error_code) ,ServerFrameworkException (SslException::error_code)
{
    Init (SslException::error_code, msg);
}
ServerNotOpenException::ServerNotOpenException () : XdbBaseException (ServerNotOpenException::error_code) ,ServerFrameworkException (ServerNotOpenException::error_code)
{
    Init (ServerNotOpenException::error_code);
}
ServerNotOpenException::ServerNotOpenException (const char* msg) : XdbBaseException (ServerNotOpenException::error_code) ,ServerFrameworkException (ServerNotOpenException::error_code)
{
    Init (ServerNotOpenException::error_code, msg);
}

NetTimeOutException::NetTimeOutException () : XdbBaseException (NetTimeOutException::error_code) ,ServerFrameworkException (NetTimeOutException::error_code)
{
    Init (NetTimeOutException::error_code);
}

NetTimeOutException::NetTimeOutException (const char* msg) : XdbBaseException (NetTimeOutException::error_code) ,ServerFrameworkException (NetTimeOutException::error_code)
{
    Init (NetTimeOutException::error_code, msg);
}

InvalidCertificateException::InvalidCertificateException () :XdbBaseException (InvalidCertificateException::error_code) ,ServerFrameworkException (InvalidCertificateException::error_code)
{
    Init (InvalidCertificateException::error_code) ;
} 

InvalidCertificateException::InvalidCertificateException (const char* msg) :XdbBaseException (InvalidCertificateException::error_code) ,ServerFrameworkException (InvalidCertificateException::error_code)
{
    Init (InvalidCertificateException::error_code, msg) ;
} 

UnkownCommandException::UnkownCommandException () :  XdbBaseException (UnkownCommandException::error_code) ,ServerFrameworkException (UnkownCommandException::error_code) 
{
    Init (UnkownCommandException::error_code) ;
}

UnkownCommandException::UnkownCommandException (const char* msg) :  XdbBaseException (UnkownCommandException::error_code) ,ServerFrameworkException (UnkownCommandException::error_code)
{
    Init (UnkownCommandException::error_code, msg) ;
}

ConnectionClosedException::ConnectionClosedException () :  XdbBaseException (ConnectionClosedException::error_code) ,ServerFrameworkException (ConnectionClosedException::error_code)
{
    Init (ConnectionClosedException::error_code) ;
}

ConnectionClosedException::ConnectionClosedException (const char* msg) :  XdbBaseException (ConnectionClosedException::error_code) ,ServerFrameworkException (ConnectionClosedException::error_code)
{
    Init (ConnectionClosedException::error_code, msg) ;
}

NoTransactionException::NoTransactionException () :  XdbBaseException (NoTransactionException::error_code) ,ServerFrameworkException (NoTransactionException::error_code)
{
    Init (NoTransactionException::error_code) ;
}

NoTransactionException::NoTransactionException (const char* msg) :  XdbBaseException (NoTransactionException::error_code) ,ServerFrameworkException (NoTransactionException::error_code)
{
    Init (NoTransactionException::error_code, msg) ;
}

TransactionExistException::TransactionExistException () :  XdbBaseException (TransactionExistException::error_code) ,ServerFrameworkException (TransactionExistException::error_code)
{
    Init (TransactionExistException::error_code) ;
}

TransactionExistException::TransactionExistException (const char* msg) :  XdbBaseException (TransactionExistException::error_code) ,ServerFrameworkException (TransactionExistException::error_code)
{
    Init (TransactionExistException::error_code, msg) ;
}

DataNodeDownException::DataNodeDownException () :  XdbBaseException (DataNodeDownException::error_code) ,ServerFrameworkException (DataNodeDownException::error_code)
{
    Init (DataNodeDownException::error_code) ;
}

DataNodeDownException::DataNodeDownException (const char* msg) :  XdbBaseException (DataNodeDownException::error_code) ,ServerFrameworkException (DataNodeDownException::error_code)
{
    Init (DataNodeDownException::error_code, msg) ;
}

ClusterEnvironmentException::ClusterEnvironmentException () :  XdbBaseException (ClusterEnvironmentException::error_code) ,ServerFrameworkException (ClusterEnvironmentException::error_code)
{
    Init (ClusterEnvironmentException::error_code) ;
}

ClusterEnvironmentException::ClusterEnvironmentException (const char* msg) :  XdbBaseException (ClusterEnvironmentException::error_code) ,ServerFrameworkException (ClusterEnvironmentException::error_code)
{
    Init (ClusterEnvironmentException::error_code, msg) ;
}


ExecutorCreateException::ExecutorCreateException () :  XdbBaseException (ExecutorCreateException::error_code) ,ServerFrameworkException (ExecutorCreateException::error_code)
{
    Init (ExecutorCreateException::error_code) ;
}

ExecutorCreateException::ExecutorCreateException (const char* msg) :  XdbBaseException (ExecutorCreateException::error_code) ,ServerFrameworkException (ExecutorCreateException::error_code)
{
    Init (ExecutorCreateException::error_code, msg) ;
}

NotLoginExeception::NotLoginExeception () :  XdbBaseException (NotLoginExeception::error_code) ,ServerFrameworkException (NotLoginExeception::error_code)
{
    Init (NotLoginExeception::error_code);
}

NotLoginExeception::NotLoginExeception (const char* msg) :  XdbBaseException (NotLoginExeception::error_code) ,ServerFrameworkException (NotLoginExeception::error_code)
{
    Init (NotLoginExeception::error_code, msg);
}

NotRecursiveLogin::NotRecursiveLogin () :  XdbBaseException (NotRecursiveLogin::error_code) ,ServerFrameworkException (NotRecursiveLogin::error_code)
{
    Init (NotRecursiveLogin::error_code);
}

NotRecursiveLogin::NotRecursiveLogin (const char* msg) :  XdbBaseException (NotRecursiveLogin::error_code) ,ServerFrameworkException (NotRecursiveLogin::error_code)
{
    Init (NotRecursiveLogin::error_code, msg);
}

SocketException::SocketException () :  XdbBaseException (SocketException::error_code) ,ServerFrameworkException (SocketException::error_code)
{
    Init (SocketException::error_code) ;
}

SocketException::SocketException (const char* msg) :  XdbBaseException (SocketException::error_code) ,ServerFrameworkException (SocketException::error_code)
{
    Init (SocketException::error_code, msg) ;
}

SocketException::SocketException (const int val, const char* msg) :  XdbBaseException (SocketException::error_code) ,ServerFrameworkException (SocketException::error_code)
{
    memset (m_infobuf, 0, sizeof(m_infobuf));
    const char* error_info = XdbBaseException::GetErrorMessage (error_code); 
    snprintf(m_infobuf, EXCEPTION_INFO_BUFLEN - 1, XDBSTR("%s|value:%d, message:%s"), error_info, val, msg);
}

SrvcfgFileException::SrvcfgFileException () :  XdbBaseException (SrvcfgFileException::error_code) ,ServerFrameworkException (SrvcfgFileException::error_code)
{
	Init (SrvcfgFileException::error_code) ;
}

SrvcfgFileException::SrvcfgFileException (const char* msg) :  XdbBaseException (SrvcfgFileException::error_code) ,ServerFrameworkException (SrvcfgFileException::error_code)
{
	Init (SrvcfgFileException::error_code, msg) ;
}

SrvcfgFileException::SrvcfgFileException (const int val, const char* msg) :  XdbBaseException (SrvcfgFileException::error_code) ,ServerFrameworkException (SrvcfgFileException::error_code)
{
	memset (m_infobuf, 0, sizeof(m_infobuf));
	const char* error_info = XdbBaseException::GetErrorMessage (error_code); 
	snprintf(m_infobuf, EXCEPTION_INFO_BUFLEN - 1, XDBSTR("%s|value:%d, message:%s"), error_info, val, msg);
}

/**********************ExecutiionEngineExpection Definition Start********************************/

//throws this exception, OpenContainerException, when do openning container operation.
OpenContainerException::OpenContainerException () : XdbBaseException (OpenContainerException::error_code),ExecutionEngineException (OpenContainerException::error_code)
{
    Init (OpenContainerException::error_code);
} 
OpenContainerException::OpenContainerException (const char* msg) : XdbBaseException (OpenContainerException::error_code),ExecutionEngineException (OpenContainerException::error_code)
{
    Init (OpenContainerException::error_code, msg);
}

//throws ContainerNotExistException when the container does not exist.
ContainerNotExistException::ContainerNotExistException () : XdbBaseException (ContainerNotExistException::error_code) ,ExecutionEngineException (ContainerNotExistException::error_code)
{
    Init (ContainerNotExistException::error_code);
} 
ContainerNotExistException::ContainerNotExistException (const char* msg) : XdbBaseException (ContainerNotExistException::error_code) ,ExecutionEngineException (ContainerNotExistException::error_code)
{
    Init (ContainerNotExistException::error_code, msg);
} 

ReIndexException::ReIndexException () : XdbBaseException (ReIndexException::error_code) ,ExecutionEngineException (ReIndexException::error_code)
{
    Init (ReIndexException::error_code);
} 
ReIndexException::ReIndexException (const char* msg) : XdbBaseException (ReIndexException::error_code) ,ExecutionEngineException (ReIndexException::error_code)
{
    Init (ReIndexException::error_code, msg);
} 

//throws GrantNodeErroException when the container does not exist.
GrantNodeErroException::GrantNodeErroException () : XdbBaseException (GrantNodeErroException::error_code) ,ExecutionEngineException (GrantNodeErroException::error_code)
{
    Init (GrantNodeErroException::error_code);
} 
GrantNodeErroException::GrantNodeErroException (const char* msg) : XdbBaseException (GrantNodeErroException::error_code) ,ExecutionEngineException (GrantNodeErroException::error_code)
{
    Init (GrantNodeErroException::error_code, msg);
} 

//throws CheckNodePrivErroException when the container does not exist.
CheckNodePrivErroException::CheckNodePrivErroException () : XdbBaseException (CheckNodePrivErroException::error_code) ,ExecutionEngineException (CheckNodePrivErroException::error_code)
{
    Init (CheckNodePrivErroException::error_code);
} 
CheckNodePrivErroException::CheckNodePrivErroException (const char* msg) : XdbBaseException (CheckNodePrivErroException::error_code) ,ExecutionEngineException (CheckNodePrivErroException::error_code)
{
    Init (CheckNodePrivErroException::error_code, msg);
} 
//throws ListNodePrivErroException when the container does not exist.
ListNodePrivErroException::ListNodePrivErroException () : XdbBaseException (ListNodePrivErroException::error_code) ,ExecutionEngineException (ListNodePrivErroException::error_code)
{
    Init (ListNodePrivErroException::error_code);
} 
ListNodePrivErroException::ListNodePrivErroException (const char* msg) : XdbBaseException (ListNodePrivErroException::error_code) ,ExecutionEngineException (ListNodePrivErroException::error_code)
{
    Init (ListNodePrivErroException::error_code, msg);
} 
//throws RevokeNodeErroException when the container does not exist.
RevokeNodeErroException::RevokeNodeErroException () : XdbBaseException (RevokeNodeErroException::error_code) ,ExecutionEngineException (RevokeNodeErroException::error_code)
{
    Init (RevokeNodeErroException::error_code);
} 
RevokeNodeErroException::RevokeNodeErroException (const char* msg) : XdbBaseException (RevokeNodeErroException::error_code) ,ExecutionEngineException (RevokeNodeErroException::error_code)
{
    Init (RevokeNodeErroException::error_code, msg);
} 

//throws ContainerNotCommitException when the container does not commit.
ContainerNotCommitException::ContainerNotCommitException () : XdbBaseException (ContainerNotCommitException::error_code) ,ExecutionEngineException (ContainerNotCommitException::error_code)
{
    Init (ContainerNotCommitException::error_code);
} 
ContainerNotCommitException::ContainerNotCommitException (const char* msg) : XdbBaseException (ContainerNotCommitException::error_code) ,ExecutionEngineException (ContainerNotCommitException::error_code)
{
    Init (ContainerNotCommitException::error_code, msg);
} 

//throws IDOverFlowException when ID over flow.
IDOverFlowException::IDOverFlowException () : XdbBaseException (IDOverFlowException::error_code) ,ExecutionEngineException (IDOverFlowException::error_code)
{
    Init (IDOverFlowException::error_code);
} 
IDOverFlowException::IDOverFlowException (const char* msg) : XdbBaseException (IDOverFlowException::error_code) ,ExecutionEngineException (IDOverFlowException::error_code)
{
    Init (IDOverFlowException::error_code, msg);
} 

//throws LargeObjectNotExistException when the large object does not exist.
LargeObjectNotExistException::LargeObjectNotExistException () : XdbBaseException (LargeObjectNotExistException::error_code) ,ExecutionEngineException (LargeObjectNotExistException::error_code)
{
    Init (LargeObjectNotExistException::error_code);
} 
LargeObjectNotExistException::LargeObjectNotExistException (const char* msg) : XdbBaseException (LargeObjectNotExistException::error_code) ,ExecutionEngineException (LargeObjectNotExistException::error_code)
{
    Init (LargeObjectNotExistException::error_code, msg);
} 

//throws LargeObjectException when the large object does not exist.
LargeObjectException::LargeObjectException () : XdbBaseException (LargeObjectException::error_code) ,ExecutionEngineException (LargeObjectException::error_code)
{
    Init (LargeObjectException::error_code);
} 
LargeObjectException::LargeObjectException (const char* msg) : XdbBaseException (LargeObjectException::error_code) ,ExecutionEngineException (LargeObjectException::error_code)
{
    Init (LargeObjectException::error_code, msg);
} 

//throws AddDocumentException when adding a doucment.
AddDocumentException::AddDocumentException () : XdbBaseException (AddDocumentException::error_code) ,ExecutionEngineException (AddDocumentException::error_code)
{
    Init (AddDocumentException::error_code);
}     

AddDocumentException::AddDocumentException (const char* msg) : XdbBaseException (AddDocumentException::error_code) ,ExecutionEngineException (AddDocumentException::error_code)
{
    Init (AddDocumentException::error_code, msg);
}   

//throws TableSpaceNotExistException when tablespace is not exit.
TableSpaceNotExistException::TableSpaceNotExistException () : XdbBaseException (TableSpaceNotExistException::error_code) , ExecutionEngineException (TableSpaceNotExistException::error_code)
{
    Init (TableSpaceNotExistException::error_code);
}
TableSpaceNotExistException::TableSpaceNotExistException (const char* msg) : XdbBaseException (TableSpaceNotExistException::error_code) , ExecutionEngineException (TableSpaceNotExistException::error_code)
{
    Init (TableSpaceNotExistException::error_code, msg);
}

//throws CreateContainerException when creating a container.
CreateContainerException::CreateContainerException () : XdbBaseException (CreateContainerException::error_code) ,ExecutionEngineException (CreateContainerException::error_code)
{
    Init (CreateContainerException::error_code);
}
CreateContainerException::CreateContainerException (const char* msg) : XdbBaseException (CreateContainerException::error_code) ,ExecutionEngineException (CreateContainerException::error_code)
{
    Init (CreateContainerException::error_code, msg);
}

DumpContainerException::DumpContainerException () : XdbBaseException (DumpContainerException::error_code) ,ExecutionEngineException (DumpContainerException::error_code)
{
    Init (DumpContainerException::error_code);
}
DumpContainerException::DumpContainerException (const char* msg) : XdbBaseException (DumpContainerException::error_code) ,ExecutionEngineException (DumpContainerException::error_code)
{
    Init (DumpContainerException::error_code, msg);
}

LoadContainerException::LoadContainerException () : XdbBaseException (LoadContainerException::error_code) ,ExecutionEngineException (LoadContainerException::error_code)
{
    Init (LoadContainerException::error_code);
}
LoadContainerException::LoadContainerException (const char* msg) : XdbBaseException (LoadContainerException::error_code) ,ExecutionEngineException (LoadContainerException::error_code)
{
    Init (LoadContainerException::error_code, msg);
}

//throws QueryStatementException if the the query statements are invalid.
QueryStatementException::QueryStatementException () : XdbBaseException (QueryStatementException::error_code) ,ExecutionEngineException (QueryStatementException::error_code)
{
    Init (QueryStatementException::error_code);
}
QueryStatementException::QueryStatementException (const char* msg) : XdbBaseException (QueryStatementException::error_code) ,ExecutionEngineException (QueryStatementException::error_code)
{
    Init (QueryStatementException::error_code,msg);
}

//throws RemoveContainerException when removing the  
RemoveContainerException::RemoveContainerException () : XdbBaseException (RemoveContainerException::error_code) ,ExecutionEngineException (RemoveContainerException::error_code)
{
    Init (RemoveContainerException::error_code);
} 
RemoveContainerException::RemoveContainerException (const char* msg) : XdbBaseException (RemoveContainerException::error_code) ,ExecutionEngineException (RemoveContainerException::error_code)
{
    Init (RemoveContainerException::error_code,msg);
} 
 
//
ContainerAlreadyExistException::ContainerAlreadyExistException () : XdbBaseException (ContainerAlreadyExistException::error_code) ,ExecutionEngineException (ContainerAlreadyExistException::error_code)
{
    Init (ContainerAlreadyExistException::error_code);
}
ContainerAlreadyExistException::ContainerAlreadyExistException (const char* msg) : XdbBaseException (ContainerAlreadyExistException::error_code) ,ExecutionEngineException (ContainerAlreadyExistException::error_code)
{
    Init (ContainerAlreadyExistException::error_code, msg);
}

//
ContainerFullException::ContainerFullException () : XdbBaseException (ContainerFullException::error_code) ,ExecutionEngineException (ContainerFullException::error_code)
{
    Init (ContainerFullException::error_code);
}
ContainerFullException::ContainerFullException (const char* msg) : XdbBaseException (ContainerFullException::error_code) ,ExecutionEngineException (ContainerFullException::error_code)
{
    Init (ContainerFullException::error_code, msg);
}

//
UpdateDocumentsException::UpdateDocumentsException () : XdbBaseException (UpdateDocumentsException::error_code) ,ExecutionEngineException (UpdateDocumentsException::error_code)
{
    Init (UpdateDocumentsException::error_code);
}
UpdateDocumentsException::UpdateDocumentsException (const char* msg) : XdbBaseException (UpdateDocumentsException::error_code) ,ExecutionEngineException (UpdateDocumentsException::error_code)
{
    Init (UpdateDocumentsException::error_code,msg);
}

//
RemoveDocumentException::RemoveDocumentException () : XdbBaseException (RemoveDocumentException::error_code) ,ExecutionEngineException (RemoveDocumentException::error_code)
{
    Init (RemoveDocumentException::error_code);
}
RemoveDocumentException::RemoveDocumentException (const char* msg) : XdbBaseException (RemoveDocumentException::error_code) ,ExecutionEngineException (RemoveDocumentException::error_code)
{
    Init (RemoveDocumentException::error_code, msg); 
}

RenameContainerException::RenameContainerException () : XdbBaseException (RenameContainerException::error_code) ,ExecutionEngineException (RenameContainerException::error_code)
{
    Init (RenameContainerException::error_code);
}
RenameContainerException::RenameContainerException (const char* msg) : XdbBaseException (RenameContainerException::error_code) ,ExecutionEngineException (RenameContainerException::error_code)
{
    Init (RenameContainerException::error_code, msg); 
}

CanNotOpenContainerException::CanNotOpenContainerException (): XdbBaseException (CanNotOpenContainerException::error_code) ,ExecutionEngineException (CanNotOpenContainerException::error_code)
{
    Init (CanNotOpenContainerException::error_code);
}       

CanNotOpenContainerException::CanNotOpenContainerException (const char* msg): XdbBaseException (CanNotOpenContainerException::error_code) ,ExecutionEngineException (CanNotOpenContainerException::error_code)
{
    Init (CanNotOpenContainerException::error_code, msg) ;
}

SqlDatabaseException::SqlDatabaseException (): XdbBaseException (SqlDatabaseException::error_code) ,ExecutionEngineException (SqlDatabaseException::error_code)
{
    Init (SqlDatabaseException::error_code);
}       

SqlDatabaseException::SqlDatabaseException (const char* msg): XdbBaseException (SqlDatabaseException::error_code) ,ExecutionEngineException (SqlDatabaseException::error_code)
{
    Init (SqlDatabaseException::error_code, msg) ;
}

AddIndexException::AddIndexException (): XdbBaseException (AddIndexException::error_code) ,ExecutionEngineException (AddIndexException::error_code)
{

    Init (AddIndexException::error_code);
}       

AddIndexException::AddIndexException (const char* msg): XdbBaseException (AddIndexException::error_code) ,ExecutionEngineException (AddIndexException::error_code)
{
    Init (AddIndexException::error_code, msg);
}

TruncateException::TruncateException (): XdbBaseException (TruncateException::error_code) , ExecutionEngineException(TruncateException::error_code)
{
    Init (TruncateException::error_code);
}

TruncateException::TruncateException (const char* msg): XdbBaseException (TruncateException::error_code) , ExecutionEngineException(TruncateException::error_code)
{
    Init (TruncateException::error_code, msg);
}

RemoveIndexException::RemoveIndexException (): XdbBaseException (RemoveIndexException::error_code) ,ExecutionEngineException (RemoveIndexException::error_code)
{
    Init (RemoveIndexException::error_code);
}       

RemoveIndexException::RemoveIndexException (const char* msg): XdbBaseException (RemoveIndexException::error_code) ,ExecutionEngineException (RemoveIndexException::error_code)
{
    Init (RemoveIndexException::error_code, msg);
}

NoPathIndexException::NoPathIndexException (): XdbBaseException (NoPathIndexException::error_code) ,ExecutionEngineException (NoPathIndexException::error_code)
{
    Init (NoPathIndexException::error_code);
}   
  
NoPathIndexException::NoPathIndexException (const char* msg): XdbBaseException (NoPathIndexException::error_code) ,ExecutionEngineException (NoPathIndexException::error_code)
{
    Init (NoPathIndexException::error_code, msg);
}
PathIndexFailException::PathIndexFailException (): XdbBaseException (PathIndexFailException::error_code) ,ExecutionEngineException (PathIndexFailException::error_code)
{
    Init (PathIndexFailException::error_code);
} 
PathIndexFailException::PathIndexFailException (const char* msg): XdbBaseException (PathIndexFailException::error_code) ,ExecutionEngineException (PathIndexFailException::error_code)
{
    Init (PathIndexFailException::error_code, msg);
} 

PathIndexRepeatException::PathIndexRepeatException (): XdbBaseException (PathIndexRepeatException::error_code) ,ExecutionEngineException (PathIndexRepeatException::error_code)
{
    Init (PathIndexRepeatException::error_code);
} 
PathIndexRepeatException::PathIndexRepeatException (const char* msg): XdbBaseException (PathIndexRepeatException::error_code) ,ExecutionEngineException (PathIndexRepeatException::error_code)
{
    Init (PathIndexRepeatException::error_code, msg);
}       

HavePathIndexException::HavePathIndexException (): XdbBaseException (HavePathIndexException::error_code) ,ExecutionEngineException (HavePathIndexException::error_code)
{
    Init (HavePathIndexException::error_code);
}       

HavePathIndexException::HavePathIndexException (const char* msg): XdbBaseException (HavePathIndexException::error_code) ,ExecutionEngineException (HavePathIndexException::error_code)
{
    Init (HavePathIndexException::error_code, msg);
}

PSExecuteException::PSExecuteException (): XdbBaseException (PSExecuteException::error_code) ,ExecutionEngineException (PSExecuteException::error_code)
{
    Init (PSExecuteException::error_code) ;
}       

PSExecuteException::PSExecuteException (const char* msg): XdbBaseException (PSExecuteException::error_code) ,ExecutionEngineException (PSExecuteException::error_code)
{
    Init (PSExecuteException::error_code, msg);
}

ParameterInvalidException::ParameterInvalidException (): XdbBaseException (ParameterInvalidException::error_code) ,ExecutionEngineException (ParameterInvalidException::error_code)
{
    Init (ParameterInvalidException::error_code) ;
}       

ParameterInvalidException::ParameterInvalidException (const char* msg): XdbBaseException (ParameterInvalidException::error_code) ,ExecutionEngineException (ParameterInvalidException::error_code)
{
    Init (ParameterInvalidException::error_code, msg);
}

CanNotOpenSystemDBException::CanNotOpenSystemDBException (): XdbBaseException (CanNotOpenSystemDBException::error_code) ,ExecutionEngineException (CanNotOpenSystemDBException::error_code)
{
    Init (CanNotOpenSystemDBException::error_code);
}       

CanNotOpenSystemDBException::CanNotOpenSystemDBException (const char* msg): XdbBaseException (CanNotOpenSystemDBException::error_code) ,ExecutionEngineException (CanNotOpenSystemDBException::error_code)
{
    Init (CanNotOpenSystemDBException::error_code,msg);
}

CanNotAccessSystemDBException::CanNotAccessSystemDBException (): XdbBaseException (CanNotAccessSystemDBException::error_code) ,ExecutionEngineException (CanNotAccessSystemDBException::error_code)
{
    Init (CanNotAccessSystemDBException::error_code);
}       

CanNotAccessSystemDBException::CanNotAccessSystemDBException (const char* msg): XdbBaseException (CanNotAccessSystemDBException::error_code) ,ExecutionEngineException (CanNotAccessSystemDBException::error_code)
{
    Init (CanNotAccessSystemDBException::error_code, msg);
}

DatabaseNamedSyntaxException::DatabaseNamedSyntaxException (): XdbBaseException (DatabaseNamedSyntaxException::error_code) ,ExecutionEngineException (DatabaseNamedSyntaxException::error_code)
{
    Init (DatabaseNamedSyntaxException::error_code);
}       

DatabaseNamedSyntaxException::DatabaseNamedSyntaxException (const char* msg): XdbBaseException (DatabaseNamedSyntaxException::error_code) ,ExecutionEngineException (DatabaseNamedSyntaxException::error_code)
{
    Init (DatabaseNamedSyntaxException::error_code,msg);
}

DatabaseUsedException::DatabaseUsedException (): XdbBaseException (DatabaseUsedException::error_code) ,ExecutionEngineException (DatabaseUsedException::error_code)
{
    Init (DatabaseUsedException::error_code);
}       

DatabaseUsedException::DatabaseUsedException (const char* msg): XdbBaseException (DatabaseUsedException::error_code) ,ExecutionEngineException (DatabaseUsedException::error_code)
{
    Init (DatabaseUsedException::error_code,msg);
}

DatabaseAlreadyExistException::DatabaseAlreadyExistException (): XdbBaseException (DatabaseAlreadyExistException::error_code) ,ExecutionEngineException (DatabaseAlreadyExistException::error_code)
{
    Init (DatabaseAlreadyExistException::error_code);
}       

DatabaseAlreadyExistException::DatabaseAlreadyExistException (const char* msg): XdbBaseException (DatabaseAlreadyExistException::error_code) ,ExecutionEngineException (DatabaseAlreadyExistException::error_code)
{
    Init (DatabaseAlreadyExistException::error_code,msg);
}
ObjectAlreadyExistException::ObjectAlreadyExistException (): XdbBaseException (ObjectAlreadyExistException::error_code) ,ExecutionEngineException (ObjectAlreadyExistException::error_code)
{
    Init (ObjectAlreadyExistException::error_code);
}   
 
ObjectAlreadyExistException::ObjectAlreadyExistException (const char* msg): XdbBaseException (ObjectAlreadyExistException::error_code) ,ExecutionEngineException (ObjectAlreadyExistException::error_code)
{
    Init (ObjectAlreadyExistException::error_code,msg);
}

ObjectNotExistException::ObjectNotExistException (): XdbBaseException (ObjectNotExistException::error_code) ,ExecutionEngineException (ObjectNotExistException::error_code)
{
    Init (ObjectNotExistException::error_code);
}  

ObjectNotExistException::ObjectNotExistException (const char* msg): XdbBaseException (ObjectNotExistException::error_code) ,ExecutionEngineException (ObjectNotExistException::error_code)
{
    Init (ObjectNotExistException::error_code,msg);
}

UserAlreadyExistException::UserAlreadyExistException (): XdbBaseException (UserAlreadyExistException::error_code) ,ExecutionEngineException (UserAlreadyExistException::error_code)
{
    Init (UserAlreadyExistException::error_code);
}       

UserAlreadyExistException::UserAlreadyExistException (const char* msg): XdbBaseException (UserAlreadyExistException::error_code) ,ExecutionEngineException (UserAlreadyExistException::error_code)
{
    Init (UserAlreadyExistException::error_code, msg);
}

CheckItemRelationException::CheckItemRelationException (): XdbBaseException (CheckItemRelationException::error_code) ,ExecutionEngineException (CheckItemRelationException::error_code)
{
    Init (CheckItemRelationException::error_code);
}       

CheckItemRelationException::CheckItemRelationException (const char* msg): XdbBaseException (CheckItemRelationException::error_code) ,ExecutionEngineException (CheckItemRelationException::error_code)
{
    Init (CheckItemRelationException::error_code, msg);
}
UserNotExistException::UserNotExistException (): XdbBaseException (UserNotExistException::error_code) ,ExecutionEngineException (UserNotExistException::error_code)
{
    Init (UserNotExistException::error_code);
}       

UserNotExistException::UserNotExistException (const char* msg): XdbBaseException (UserNotExistException::error_code) ,ExecutionEngineException (UserNotExistException::error_code)
{
    Init (UserNotExistException::error_code,msg);
}

PasswordIncorrectException::PasswordIncorrectException (): XdbBaseException (PasswordIncorrectException::error_code) ,ExecutionEngineException (PasswordIncorrectException::error_code)
{
    Init (PasswordIncorrectException::error_code);
}   
PasswordIncorrectException::PasswordIncorrectException (const char* msg): XdbBaseException (PasswordIncorrectException::error_code) ,ExecutionEngineException (PasswordIncorrectException::error_code)
{
    Init (PasswordIncorrectException::error_code, msg);
}    
UserRoleIsNotAdminException::UserRoleIsNotAdminException (): XdbBaseException (UserRoleIsNotAdminException::error_code) ,ExecutionEngineException (UserRoleIsNotAdminException::error_code)
{
    Init (UserRoleIsNotAdminException::error_code);
}
UserRoleIsNotAdminException::UserRoleIsNotAdminException (const char* msg): XdbBaseException (UserRoleIsNotAdminException::error_code) ,ExecutionEngineException (UserRoleIsNotAdminException::error_code)
{
    Init (UserRoleIsNotAdminException::error_code, msg);
}

SequenceErroException::SequenceErroException (): XdbBaseException (SequenceErroException::error_code) ,ExecutionEngineException (SequenceErroException::error_code)
{
    Init (SequenceErroException::error_code);
}
SequenceErroException::SequenceErroException (const char* msg): XdbBaseException (SequenceErroException::error_code) ,ExecutionEngineException (SequenceErroException::error_code)
{
    Init (SequenceErroException::error_code, msg);
}

NewPasswdSameWithOldPasswdException::NewPasswdSameWithOldPasswdException (): XdbBaseException (NewPasswdSameWithOldPasswdException::error_code) ,ExecutionEngineException (NewPasswdSameWithOldPasswdException::error_code)
{
	Init (NewPasswdSameWithOldPasswdException::error_code);
}       

NewPasswdSameWithOldPasswdException::NewPasswdSameWithOldPasswdException (const char* msg): XdbBaseException (NewPasswdSameWithOldPasswdException::error_code) ,ExecutionEngineException (NewPasswdSameWithOldPasswdException::error_code)
{
	Init (NewPasswdSameWithOldPasswdException::error_code, msg);
}

PreparedStatementNotExist::PreparedStatementNotExist (): XdbBaseException (PreparedStatementNotExist::error_code) ,ExecutionEngineException (PreparedStatementNotExist::error_code)
{
    memset (m_infobuf, 0, sizeof(m_infobuf));
    const char* error_info = XdbBaseException::GetErrorMessage (error_code); 
    uint32 length = ((uint32)strlen (error_info) > EXCEPTION_INFO_BUFLEN )? EXCEPTION_INFO_BUFLEN : (uint32)strlen (error_info) ;
    strncpy (m_infobuf, error_info, length);          
}       

PreparedStatementNotExist::PreparedStatementNotExist (const char* msg): XdbBaseException (PreparedStatementNotExist::error_code) ,ExecutionEngineException (PreparedStatementNotExist::error_code)
{
    memset (m_infobuf, 0, sizeof(m_infobuf));
    const char* error_info = XdbBaseException::GetErrorMessage (error_code); 
    uint32 length = ((uint32)strlen (error_info) > EXCEPTION_INFO_BUFLEN )? EXCEPTION_INFO_BUFLEN : (uint32)strlen (error_info) ;
    strncpy (m_infobuf, error_info, length);  
    
    
    if (strlen(m_infobuf)  < EXCEPTION_INFO_BUFLEN) {
        strncat (m_infobuf, "|", strlen ("|"));
        strncat (m_infobuf, msg, strlen (msg));       
    }        
}

RoleAlreadyExistException::RoleAlreadyExistException (): XdbBaseException (RoleAlreadyExistException::error_code), ExecutionEngineException (RoleAlreadyExistException::error_code)
{
    Init (RoleAlreadyExistException::error_code);
}       

RoleAlreadyExistException::RoleAlreadyExistException (const char* msg): XdbBaseException (RoleAlreadyExistException::error_code) ,ExecutionEngineException (RoleAlreadyExistException::error_code) 
{
    Init (RoleAlreadyExistException::error_code,msg);
}

RoleNotExistException::RoleNotExistException (): XdbBaseException (RoleNotExistException::error_code) ,ExecutionEngineException (RoleNotExistException::error_code)
{
    Init (RoleNotExistException::error_code);
}       

RoleNotExistException::RoleNotExistException (const char* msg): XdbBaseException (RoleNotExistException::error_code) ,ExecutionEngineException (RoleNotExistException::error_code)
{
    Init (RoleNotExistException::error_code, msg);
}

CanNotBindUserNameException::CanNotBindUserNameException (): XdbBaseException (CanNotBindUserNameException::error_code) ,ExecutionEngineException (CanNotBindUserNameException::error_code)
{
    Init (CanNotBindUserNameException::error_code);
}       

CanNotBindUserNameException::CanNotBindUserNameException (const char* msg): XdbBaseException (CanNotBindUserNameException::error_code) ,ExecutionEngineException (CanNotBindUserNameException::error_code)
{
    Init (CanNotBindUserNameException::error_code, msg);
}

CanNotDeleteSystemTableUserException::CanNotDeleteSystemTableUserException (): XdbBaseException (CanNotDeleteSystemTableUserException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableUserException::error_code)
{
    Init (CanNotDeleteSystemTableUserException::error_code);
}       

CanNotDeleteSystemTableUserException::CanNotDeleteSystemTableUserException (const char* msg): XdbBaseException (CanNotDeleteSystemTableUserException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableUserException::error_code)
{
    Init (CanNotDeleteSystemTableUserException::error_code, msg);
}

CanNotBindUserIDException::CanNotBindUserIDException (): XdbBaseException (CanNotBindUserIDException::error_code) ,ExecutionEngineException (CanNotBindUserIDException::error_code)
{   
    Init (CanNotBindUserIDException::error_code);
}       

CanNotBindUserIDException::CanNotBindUserIDException (const char* msg): XdbBaseException (CanNotBindUserIDException::error_code) ,ExecutionEngineException (CanNotBindUserIDException::error_code)
{
    Init (CanNotBindUserIDException::error_code, msg);
}

CanNotDeleteSystemTableUserPrivilegeException::CanNotDeleteSystemTableUserPrivilegeException (): XdbBaseException (CanNotDeleteSystemTableUserPrivilegeException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableUserPrivilegeException::error_code)
{
    Init (CanNotDeleteSystemTableUserPrivilegeException::error_code);
}       

CanNotDeleteSystemTableUserPrivilegeException::CanNotDeleteSystemTableUserPrivilegeException (const char* msg): XdbBaseException (CanNotDeleteSystemTableUserPrivilegeException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableUserPrivilegeException::error_code)
{
    Init (CanNotDeleteSystemTableUserPrivilegeException::error_code, msg);
}

CanNotBindUserIDFromRoleUserException::CanNotBindUserIDFromRoleUserException (): XdbBaseException (CanNotBindUserIDFromRoleUserException::error_code) ,ExecutionEngineException (CanNotBindUserIDFromRoleUserException::error_code)
{
    Init (CanNotBindUserIDFromRoleUserException::error_code);
}       

CanNotBindUserIDFromRoleUserException::CanNotBindUserIDFromRoleUserException (const char* msg): XdbBaseException (CanNotBindUserIDFromRoleUserException::error_code) ,ExecutionEngineException (CanNotBindUserIDFromRoleUserException::error_code)
{
    Init (CanNotBindUserIDFromRoleUserException::error_code,msg);
}

CanNotDeleteSystemTableRoleUserByUserIDException::CanNotDeleteSystemTableRoleUserByUserIDException (): XdbBaseException (CanNotDeleteSystemTableRoleUserByUserIDException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRoleUserByUserIDException::error_code) 
{
    Init (CanNotDeleteSystemTableRoleUserByUserIDException::error_code);
}       

CanNotDeleteSystemTableRoleUserByUserIDException::CanNotDeleteSystemTableRoleUserByUserIDException (const char* msg): XdbBaseException (CanNotDeleteSystemTableRoleUserByUserIDException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRoleUserByUserIDException::error_code)
{
    Init (CanNotDeleteSystemTableRoleUserByUserIDException::error_code,msg);
}

//
CanNotBindRoleNameException::CanNotBindRoleNameException (): XdbBaseException (CanNotBindRoleNameException::error_code) ,ExecutionEngineException (CanNotBindRoleNameException::error_code)
{
    Init (CanNotBindRoleNameException::error_code);
}       

CanNotBindRoleNameException::CanNotBindRoleNameException (const char* msg): XdbBaseException (CanNotBindRoleNameException::error_code) ,ExecutionEngineException (CanNotBindRoleNameException::error_code)
{
    Init (CanNotBindRoleNameException::error_code,msg);
}

CanNotDeleteSystemTableRoleException::CanNotDeleteSystemTableRoleException (): XdbBaseException (CanNotDeleteSystemTableRoleException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRoleException::error_code)
{
    Init (CanNotDeleteSystemTableRoleException::error_code);
}       

CanNotDeleteSystemTableRoleException::CanNotDeleteSystemTableRoleException (const char* msg): XdbBaseException (CanNotDeleteSystemTableRoleException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRoleException::error_code)
{
    Init (CanNotDeleteSystemTableRoleException::error_code, msg);
}

CanNotBindRoleIDException::CanNotBindRoleIDException (): XdbBaseException (CanNotBindRoleIDException::error_code) ,ExecutionEngineException (CanNotBindRoleIDException::error_code)
{
   Init (CanNotBindRoleIDException::error_code);
}       

CanNotBindRoleIDException::CanNotBindRoleIDException (const char* msg): XdbBaseException (CanNotBindRoleIDException::error_code) ,ExecutionEngineException (CanNotBindRoleIDException::error_code)
{
    Init (CanNotBindRoleIDException::error_code, msg);
}

CanNotDeleteSystemTableRolePrivilegeException::CanNotDeleteSystemTableRolePrivilegeException (): XdbBaseException (CanNotDeleteSystemTableRolePrivilegeException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRolePrivilegeException::error_code)
{
    Init (CanNotDeleteSystemTableRolePrivilegeException::error_code);
}       

CanNotDeleteSystemTableRolePrivilegeException::CanNotDeleteSystemTableRolePrivilegeException (const char* msg): XdbBaseException (CanNotDeleteSystemTableRolePrivilegeException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRolePrivilegeException::error_code)
{
    Init (CanNotDeleteSystemTableRolePrivilegeException::error_code, msg);
}

CanNotBindRoleIDFromRoleUserException::CanNotBindRoleIDFromRoleUserException (): XdbBaseException (CanNotBindRoleIDFromRoleUserException::error_code) ,ExecutionEngineException (CanNotBindRoleIDFromRoleUserException::error_code)
{
    Init (CanNotBindRoleIDFromRoleUserException::error_code);      
}       

CanNotBindRoleIDFromRoleUserException::CanNotBindRoleIDFromRoleUserException (const char* msg): XdbBaseException (CanNotBindRoleIDFromRoleUserException::error_code) ,ExecutionEngineException (CanNotBindRoleIDFromRoleUserException::error_code)
{
    Init (CanNotBindRoleIDFromRoleUserException::error_code, msg);      
}

CanNotDeleteSystemTableRoleUserByRoleIDException::CanNotDeleteSystemTableRoleUserByRoleIDException (): XdbBaseException (CanNotDeleteSystemTableRoleUserByRoleIDException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRoleUserByRoleIDException::error_code)
{
    Init (CanNotDeleteSystemTableRoleUserByRoleIDException::error_code);
}       

CanNotDeleteSystemTableRoleUserByRoleIDException::CanNotDeleteSystemTableRoleUserByRoleIDException (const char* msg): XdbBaseException (CanNotDeleteSystemTableRoleUserByRoleIDException::error_code) ,ExecutionEngineException (CanNotDeleteSystemTableRoleUserByRoleIDException::error_code)
{
    Init (CanNotDeleteSystemTableRoleUserByRoleIDException::error_code,msg);
}

UserOrRoleNotExistException::UserOrRoleNotExistException (): XdbBaseException (UserOrRoleNotExistException::error_code) ,ExecutionEngineException (UserOrRoleNotExistException::error_code)
{
    Init (UserOrRoleNotExistException::error_code);
}       

UserOrRoleNotExistException::UserOrRoleNotExistException (const char* msg): XdbBaseException (UserOrRoleNotExistException::error_code) ,ExecutionEngineException (UserOrRoleNotExistException::error_code)
{
    Init (UserOrRoleNotExistException::error_code,msg);
}

ObjPrivsNotGrantedException::ObjPrivsNotGrantedException (): XdbBaseException (ObjPrivsNotGrantedException::error_code) ,ExecutionEngineException (ObjPrivsNotGrantedException::error_code)
{
    Init (ObjPrivsNotGrantedException::error_code);
}

ObjPrivsNotGrantedException::ObjPrivsNotGrantedException (const char* msg): XdbBaseException (ObjPrivsNotGrantedException::error_code) ,ExecutionEngineException (ObjPrivsNotGrantedException::error_code)
{
    Init (ObjPrivsNotGrantedException::error_code,msg);
}

SysPrivsNotGrantedException::SysPrivsNotGrantedException (): XdbBaseException (SysPrivsNotGrantedException::error_code) ,ExecutionEngineException (SysPrivsNotGrantedException::error_code)
{
    Init (SysPrivsNotGrantedException::error_code);
}

SysPrivsNotGrantedException::SysPrivsNotGrantedException (const char* msg): XdbBaseException (SysPrivsNotGrantedException::error_code) ,ExecutionEngineException (SysPrivsNotGrantedException::error_code)
{
    Init (SysPrivsNotGrantedException::error_code,msg);
}

PrivNotExistException::PrivNotExistException (): XdbBaseException (PrivNotExistException::error_code) ,ExecutionEngineException (PrivNotExistException::error_code)
{
    Init (PrivNotExistException::error_code);
}

PrivNotExistException::PrivNotExistException (const char* msg): XdbBaseException (PrivNotExistException::error_code) ,ExecutionEngineException (PrivNotExistException::error_code)
{
    Init (PrivNotExistException::error_code,msg);
}

UserNotAdminException::UserNotAdminException (): XdbBaseException (UserNotAdminException::error_code) ,ExecutionEngineException (UserNotAdminException::error_code)
{
    Init (UserNotAdminException::error_code);
}

UserNotAdminException::UserNotAdminException (const char* msg): XdbBaseException (UserNotAdminException::error_code) ,ExecutionEngineException (UserNotAdminException::error_code)
{
    Init (UserNotAdminException::error_code,msg);
}

PathNotExistException::PathNotExistException (): XdbBaseException (PathNotExistException::error_code) ,ExecutionEngineException (PathNotExistException::error_code)
{
    Init (PathNotExistException::error_code);
}

PathNotExistException::PathNotExistException (const char* msg): XdbBaseException (PathNotExistException::error_code) ,ExecutionEngineException (PathNotExistException::error_code)
{
    Init (PathNotExistException::error_code,msg);
}

PathAlreadyExistException::PathAlreadyExistException (): XdbBaseException (PathAlreadyExistException::error_code) ,ExecutionEngineException (PathAlreadyExistException::error_code)
{
    Init (PathAlreadyExistException::error_code);
}

PathAlreadyExistException::PathAlreadyExistException (const char* msg): XdbBaseException (PathAlreadyExistException::error_code) ,ExecutionEngineException (PathAlreadyExistException::error_code)
{
    Init (PathAlreadyExistException::error_code,msg);
}

GetCurrentDatabaseException::GetCurrentDatabaseException (): XdbBaseException (GetCurrentDatabaseException::error_code) ,ExecutionEngineException (GetCurrentDatabaseException::error_code)
{
    Init (GetCurrentDatabaseException::error_code);
}

GetCurrentDatabaseException::GetCurrentDatabaseException (const char* msg): XdbBaseException (GetCurrentDatabaseException::error_code) ,ExecutionEngineException (GetCurrentDatabaseException::error_code)
{
    Init (GetCurrentDatabaseException::error_code,msg);
}

GetCurrentUserException::GetCurrentUserException (): XdbBaseException (GetCurrentUserException::error_code) ,ExecutionEngineException (GetCurrentUserException::error_code)
{
    Init (GetCurrentUserException::error_code);
}

GetCurrentUserException::GetCurrentUserException (const char* msg): XdbBaseException (GetCurrentUserException::error_code) ,ExecutionEngineException (GetCurrentUserException::error_code)
{
    Init (GetCurrentUserException::error_code,msg);
}

CanNotChangeAdminUserException::CanNotChangeAdminUserException (): XdbBaseException (CanNotChangeAdminUserException::error_code) ,ExecutionEngineException (CanNotChangeAdminUserException::error_code)
{
    Init (CanNotChangeAdminUserException::error_code);
}

CanNotChangeAdminUserException::CanNotChangeAdminUserException (const char* msg): XdbBaseException (CanNotChangeAdminUserException::error_code) ,ExecutionEngineException (CanNotChangeAdminUserException::error_code)
{
    Init (CanNotChangeAdminUserException::error_code,msg);
}

CanNotChangeSysAdminRoleException::CanNotChangeSysAdminRoleException (): XdbBaseException (CanNotChangeSysAdminRoleException::error_code) ,ExecutionEngineException (CanNotChangeSysAdminRoleException::error_code)
{
    Init (CanNotChangeSysAdminRoleException::error_code);
}

CanNotChangeSysAdminRoleException::CanNotChangeSysAdminRoleException (const char* msg): XdbBaseException (CanNotChangeSysAdminRoleException::error_code) ,ExecutionEngineException (CanNotChangeSysAdminRoleException::error_code)
{
    Init (CanNotChangeSysAdminRoleException::error_code,msg);
}

OverFlowException::OverFlowException (): XdbBaseException (OverFlowException::error_code) ,ExecutionEngineException (OverFlowException::error_code)
{
    Init (OverFlowException::error_code);
}

OverFlowException::OverFlowException (const char* msg): XdbBaseException (OverFlowException::error_code) ,ExecutionEngineException (OverFlowException::error_code)
{
    Init (OverFlowException::error_code,msg);
}

/**********************ExecutiionEngineExpection Definition End********************************/

UserHasExistException::UserHasExistException (): XdbBaseException (UserHasExistException::error_code),DatabaseException (UserHasExistException::error_code)
{
    Init (UserHasExistException::error_code);
}      

UserHasExistException::UserHasExistException (const char* msg): XdbBaseException (UserHasExistException::error_code),DatabaseException (UserHasExistException::error_code)
{
    Init (UserHasExistException::error_code, msg);
}      


//throws an exception, UserNotExistsException, when performs an operation on database user, or else.  
UserNotExistsException::UserNotExistsException () :XdbBaseException (UserNotExistsException::error_code) ,DatabaseException (UserNotExistsException::error_code)
{
    Init (UserNotExistsException::error_code);
}

UserNotExistsException::UserNotExistsException (const char* msg) :XdbBaseException (UserNotExistsException::error_code) ,DatabaseException (UserNotExistsException::error_code)
{
    Init (UserNotExistsException::error_code,msg);
}

TablespaceNotExistsException::TablespaceNotExistsException () :XdbBaseException (TablespaceNotExistsException::error_code) ,DatabaseException (TablespaceNotExistsException::error_code)
{
	Init (TablespaceNotExistsException::error_code);
}

TablespaceNotExistsException::TablespaceNotExistsException (const char* msg) :XdbBaseException (TablespaceNotExistsException::error_code) ,DatabaseException (TablespaceNotExistsException::error_code)
{
	Init (TablespaceNotExistsException::error_code,msg);
}

TablespaceRemoveException::TablespaceRemoveException () :XdbBaseException (TablespaceRemoveException::error_code) ,DatabaseException (TablespaceRemoveException::error_code)
{
    Init (TablespaceRemoveException::error_code);
}

TablespaceRemoveException::TablespaceRemoveException (const char* msg) :XdbBaseException (TablespaceRemoveException::error_code) ,DatabaseException (TablespaceRemoveException::error_code)
{
    Init (TablespaceRemoveException::error_code,msg);
}

ExecuteBackupException::ExecuteBackupException () :XdbBaseException (ExecuteBackupException::error_code) ,StorageEngineException (ExecuteBackupException::error_code)
{
    Init (ExecuteBackupException::error_code);
}

ExecuteBackupException::ExecuteBackupException (const char* msg) :XdbBaseException (ExecuteBackupException::error_code) ,StorageEngineException (ExecuteBackupException::error_code)
{
    Init (ExecuteBackupException::error_code,msg);
}

ExecuteRestoreException::ExecuteRestoreException () :XdbBaseException (ExecuteRestoreException::error_code) ,StorageEngineException (ExecuteRestoreException::error_code)
{
    Init (ExecuteRestoreException::error_code);
}

ExecuteRestoreException::ExecuteRestoreException (const char* msg) :XdbBaseException (ExecuteRestoreException::error_code) ,StorageEngineException (ExecuteRestoreException::error_code)
{
    Init (ExecuteRestoreException::error_code,msg);
}

CheckSystemStatusException::CheckSystemStatusException () :XdbBaseException (CheckSystemStatusException::error_code) ,StorageEngineException (CheckSystemStatusException::error_code)
{
    Init (CheckSystemStatusException::error_code);
}

CheckSystemStatusException::CheckSystemStatusException (const char* msg) :XdbBaseException (CheckSystemStatusException::error_code) ,StorageEngineException (CheckSystemStatusException::error_code)
{
    Init (CheckSystemStatusException::error_code,msg);
}

//thows a RoleHasExistException when does operation on users' roles. Such as, create a role or drop a role.
RoleHasExistException::RoleHasExistException () :XdbBaseException (RoleHasExistException::error_code) ,DatabaseException (RoleHasExistException::error_code)
{
    Init (RoleHasExistException::error_code);
}

RoleHasExistException::RoleHasExistException (const char* msg) :XdbBaseException (RoleHasExistException::error_code) ,DatabaseException (RoleHasExistException::error_code)
{
    Init (RoleHasExistException::error_code, msg);
}

//throws a RoleNotExistsException when does operation on users' roles. Such as, create a role or drop a role.
RoleNotExistsException::RoleNotExistsException (): XdbBaseException (RoleNotExistsException::error_code) ,DatabaseException (RoleNotExistsException::error_code)
{
    Init (RoleNotExistsException::error_code);
} 

        
RoleNotExistsException::RoleNotExistsException (const char* msg): XdbBaseException (RoleNotExistsException::error_code) ,DatabaseException (RoleNotExistsException::error_code)
{
    Init (RoleNotExistsException::error_code,msg);
}  

//throws a DatabaseNotExistException when does operation on database. Such as, create remove a database.
DatabaseNotExistException::DatabaseNotExistException (): XdbBaseException (DatabaseNotExistException::error_code) ,ExecutionEngineException (DatabaseNotExistException::error_code)
{
    Init (DatabaseNotExistException::error_code);
}

DatabaseNotExistException::DatabaseNotExistException (const char* msg): XdbBaseException (DatabaseNotExistException::error_code) ,ExecutionEngineException( DatabaseNotExistException::error_code)
{
    Init (DatabaseNotExistException::error_code,msg);
}  

DatabaseDeleteFailException::DatabaseDeleteFailException (): XdbBaseException (DatabaseDeleteFailException::error_code) ,ExecutionEngineException (DatabaseDeleteFailException::error_code)
{
    Init (DatabaseDeleteFailException::error_code);
}

DatabaseDeleteFailException::DatabaseDeleteFailException (const char* msg): XdbBaseException (DatabaseDeleteFailException::error_code) ,ExecutionEngineException( DatabaseDeleteFailException::error_code)
{
    Init (DatabaseDeleteFailException::error_code,msg);
} 

CreateTableSpaceException::CreateTableSpaceException (): XdbBaseException (CreateTableSpaceException::error_code) ,ExecutionEngineException (CreateTableSpaceException::error_code)
{
    Init (CreateTableSpaceException::error_code);
}

CreateTableSpaceException::CreateTableSpaceException (const char* msg): XdbBaseException (CreateTableSpaceException::error_code) ,ExecutionEngineException( CreateTableSpaceException::error_code)
{
    Init (CreateTableSpaceException::error_code,msg);
}  

TableSpaceAlreadyExistException::TableSpaceAlreadyExistException (): XdbBaseException (TableSpaceAlreadyExistException::error_code) ,ExecutionEngineException (TableSpaceAlreadyExistException::error_code)
{
    Init (TableSpaceAlreadyExistException::error_code);
}

TableSpaceAlreadyExistException::TableSpaceAlreadyExistException (const char* msg): XdbBaseException (TableSpaceAlreadyExistException::error_code) ,ExecutionEngineException( TableSpaceAlreadyExistException::error_code)
{
    Init (TableSpaceAlreadyExistException::error_code,msg);
} 

SlaveReadOnlyException::SlaveReadOnlyException (): XdbBaseException (SlaveReadOnlyException::error_code) ,ExecutionEngineException (SlaveReadOnlyException::error_code)
{
    Init (SlaveReadOnlyException::error_code);
}

SlaveReadOnlyException::SlaveReadOnlyException (const char* msg): XdbBaseException (SlaveReadOnlyException::error_code) ,ExecutionEngineException( SlaveReadOnlyException::error_code)
{
    Init (SlaveReadOnlyException::error_code,msg);
} 

//throws this exception when the user is an un-authorized user.
NotAuthorizedException::NotAuthorizedException (): XdbBaseException (NotAuthorizedException::error_code) ,DatabaseException (NotAuthorizedException::error_code)
{
    Init (NotAuthorizedException::error_code);
}     

NotAuthorizedException::NotAuthorizedException (const char* msg): XdbBaseException (NotAuthorizedException::error_code) ,DatabaseException (NotAuthorizedException::error_code)
{
    Init (NotAuthorizedException::error_code,msg);
}     

DatabaseNotUsedException::DatabaseNotUsedException (const char* msg): XdbBaseException (DatabaseNotUsedException::error_code) ,DatabaseException (DatabaseNotUsedException::error_code)
{
    Init (DatabaseNotUsedException::error_code,msg);
}     

DatabaseNotUsedException::DatabaseNotUsedException (): XdbBaseException (DatabaseNotUsedException::error_code) ,DatabaseException (DatabaseNotUsedException::error_code)
{
    Init (DatabaseNotUsedException::error_code);
}     


CollectionAlreadyExistException::CollectionAlreadyExistException () : XdbBaseException(CollectionAlreadyExistException::error_code) , ExecutionEngineException (CollectionAlreadyExistException::error_code)
{
    Init (CollectionAlreadyExistException::error_code);
}

CollectionAlreadyExistException::CollectionAlreadyExistException (const char* msg) : XdbBaseException(CollectionAlreadyExistException::error_code) , ExecutionEngineException (CollectionAlreadyExistException::error_code)
{
    Init (CollectionAlreadyExistException::error_code,msg);
}
DocFileNotExistException::DocFileNotExistException () : XdbBaseException(DocFileNotExistException::error_code), ExecutionEngineException (DocFileNotExistException::error_code) 
{
    Init (DocFileNotExistException::error_code);
}

DocFileNotExistException::DocFileNotExistException (const char* msg) : XdbBaseException(DocFileNotExistException::error_code), ExecutionEngineException (DocFileNotExistException::error_code) 
{
    Init (DocFileNotExistException::error_code, msg);
}
DocFileAlreadyExistException::DocFileAlreadyExistException () : XdbBaseException(DocFileAlreadyExistException::error_code), ExecutionEngineException (DocFileAlreadyExistException::error_code) 
{
    Init (DocFileAlreadyExistException::error_code);
}
DocFileAlreadyExistException::DocFileAlreadyExistException (const char* msg) : XdbBaseException(DocFileAlreadyExistException::error_code), ExecutionEngineException (DocFileAlreadyExistException::error_code) 
{
    Init (DocFileAlreadyExistException::error_code, msg);
}
DeleteCollectionException::DeleteCollectionException () : XdbBaseException(DeleteCollectionException::error_code), ExecutionEngineException (DeleteCollectionException::error_code) 
{
    Init (DeleteCollectionException::error_code) ;
} 
DeleteCollectionException::DeleteCollectionException (const char* msg) : XdbBaseException(DeleteCollectionException::error_code), ExecutionEngineException (DeleteCollectionException::error_code) 
{
    Init (DeleteCollectionException::error_code, msg);
}

CollectionNotExistException::CollectionNotExistException () : XdbBaseException(CollectionNotExistException::error_code), ExecutionEngineException (CollectionNotExistException::error_code) 
{
    Init (CollectionNotExistException::error_code) ;
} 

CollectionNotExistException::CollectionNotExistException (const char* msg) : XdbBaseException(CollectionNotExistException::error_code), ExecutionEngineException (CollectionNotExistException::error_code) 
{
    Init (CollectionNotExistException::error_code, msg) ;
} 

CanNotAccessCollectionException::CanNotAccessCollectionException() :  XdbBaseException(CanNotAccessCollectionException::error_code), ExecutionEngineException (CanNotAccessCollectionException::error_code) 
{
    Init (CanNotAccessCollectionException::error_code) ;
}
CanNotAccessCollectionException::CanNotAccessCollectionException(const char* msg) :  XdbBaseException(CanNotAccessCollectionException::error_code), ExecutionEngineException (CanNotAccessCollectionException::error_code) 
{
    Init (CanNotAccessCollectionException::error_code, msg) ;
}
CanNotAccessDocFileException::CanNotAccessDocFileException () :  XdbBaseException(CanNotAccessDocFileException::error_code), ExecutionEngineException (CanNotAccessDocFileException::error_code) 
{
    Init (CanNotAccessDocFileException::error_code) ;
}
CanNotAccessDocFileException::CanNotAccessDocFileException (const char* msg) : XdbBaseException(CanNotAccessDocFileException::error_code), ExecutionEngineException (CanNotAccessDocFileException::error_code) 
{
    Init (CanNotAccessDocFileException::error_code, msg) ;
}

CheckDocItemCascadeException::CheckDocItemCascadeException () :  XdbBaseException(CheckDocItemCascadeException::error_code), ExecutionEngineException (CheckDocItemCascadeException::error_code) 
{
    Init (CheckDocItemCascadeException::error_code) ;
}
CheckDocItemCascadeException::CheckDocItemCascadeException (const char* msg) : XdbBaseException(CheckDocItemCascadeException::error_code), ExecutionEngineException (CheckDocItemCascadeException::error_code) 
{
    Init (CheckDocItemCascadeException::error_code, msg) ;
}
CanNotInsertSystemTableException::CanNotInsertSystemTableException () : XdbBaseException(CanNotInsertSystemTableException::error_code), ExecutionEngineException (CanNotInsertSystemTableException::error_code) 
{
    Init (CanNotInsertSystemTableException::error_code) ;
}

CanNotInsertSystemTableException::CanNotInsertSystemTableException (const char* msg) : XdbBaseException(CanNotInsertSystemTableException::error_code), ExecutionEngineException (CanNotInsertSystemTableException::error_code) 
{
    Init (CanNotInsertSystemTableException::error_code, msg) ;
}
CanNotDeleteSystemTableException::CanNotDeleteSystemTableException () : XdbBaseException(CanNotDeleteSystemTableException::error_code), ExecutionEngineException (CanNotDeleteSystemTableException::error_code) 
{
    Init (CanNotDeleteSystemTableException::error_code) ;
}
CanNotDeleteSystemTableException::CanNotDeleteSystemTableException (const char* msg) : XdbBaseException(CanNotDeleteSystemTableException::error_code), ExecutionEngineException (CanNotDeleteSystemTableException::error_code) 
{
    Init (CanNotDeleteSystemTableException::error_code, msg) ;
}

CanNotInsertSystemTreeTableException::CanNotInsertSystemTreeTableException () : XdbBaseException(CanNotInsertSystemTreeTableException::error_code), ExecutionEngineException (CanNotInsertSystemTreeTableException::error_code) 
{
    Init (CanNotInsertSystemTreeTableException::error_code) ;
}

CanNotInsertSystemTreeTableException::CanNotInsertSystemTreeTableException (const char* msg) : XdbBaseException(CanNotInsertSystemTreeTableException::error_code), ExecutionEngineException (CanNotInsertSystemTreeTableException::error_code) 
{
    Init (CanNotInsertSystemTreeTableException::error_code, msg) ;
}

CanNotDeleteSystemTreeTableException::CanNotDeleteSystemTreeTableException () : XdbBaseException(CanNotDeleteSystemTreeTableException::error_code), ExecutionEngineException (CanNotDeleteSystemTreeTableException::error_code) 
{
    Init (CanNotDeleteSystemTreeTableException::error_code) ;
}

CanNotDeleteSystemTreeTableException::CanNotDeleteSystemTreeTableException (const char* msg) : XdbBaseException(CanNotDeleteSystemTreeTableException::error_code), ExecutionEngineException (CanNotDeleteSystemTreeTableException::error_code) 
{
    Init (CanNotDeleteSystemTreeTableException::error_code, msg) ;
}

RelationShipNotExistException::RelationShipNotExistException () : XdbBaseException(RelationShipNotExistException::error_code), ExecutionEngineException (RelationShipNotExistException::error_code) 
{
    Init (RelationShipNotExistException::error_code);
}
RelationShipNotExistException::RelationShipNotExistException (const char* msg) : XdbBaseException(RelationShipNotExistException::error_code), ExecutionEngineException (RelationShipNotExistException::error_code) 
{
    Init (RelationShipNotExistException::error_code, msg);
}

RelationShipHasExistException::RelationShipHasExistException () : XdbBaseException(RelationShipHasExistException::error_code), ExecutionEngineException (RelationShipHasExistException::error_code) 
{
    Init (RelationShipHasExistException::error_code);
}
RelationShipHasExistException::RelationShipHasExistException (const char* msg) : XdbBaseException(RelationShipHasExistException::error_code), ExecutionEngineException (RelationShipHasExistException::error_code) 
{
    Init (RelationShipHasExistException::error_code, msg);
}

ResourceNotFoundException::ResourceNotFoundException () : XdbBaseException(ResourceNotFoundException::error_code), ExecutionEngineException (ResourceNotFoundException::error_code) 
{
    Init (ResourceNotFoundException::error_code);
}
ResourceNotFoundException::ResourceNotFoundException (const char* msg) : XdbBaseException(ResourceNotFoundException::error_code), ExecutionEngineException (ResourceNotFoundException::error_code) 
{
    Init (ResourceNotFoundException::error_code, msg);
}

InternalQueryException::InternalQueryException () : XdbBaseException(InternalQueryException::error_code), ExecutionEngineException (InternalQueryException::error_code) 
{
    Init (InternalQueryException::error_code);
}
InternalQueryException::InternalQueryException (const char* msg): XdbBaseException(InternalQueryException::error_code), ExecutionEngineException (InternalQueryException::error_code) 
{
    Init (InternalQueryException::error_code, msg);
}

NameLegalException::NameLegalException () : XdbBaseException(NameLegalException::error_code), ExecutionEngineException (NameLegalException::error_code) 
{
    Init (NameLegalException::error_code);
}
NameLegalException::NameLegalException (const char* msg): XdbBaseException(NameLegalException::error_code), ExecutionEngineException (NameLegalException::error_code) 
{
    Init (NameLegalException::error_code, msg);
}

AddressInUseException::AddressInUseException (const char* ipstr, const int& port) : XdbBaseException(AddressInUseException::error_code), ServerFrameworkException (AddressInUseException::error_code) 
{
    memset (m_infobuf, 0, sizeof(m_infobuf));
    const char* error_info = XdbBaseException::GetErrorMessage (error_code); 
    snprintf(m_infobuf, sizeof(m_infobuf), error_info, ipstr, port);
}

AddressInUseException::AddressInUseException (const char* msg) : XdbBaseException(AddressInUseException::error_code), ServerFrameworkException (AddressInUseException::error_code) 
{
    Init (AddressInUseException::error_code, msg);
}

ConversionFailedException::ConversionFailedException () :XdbBaseException (ConversionFailedException::error_code) ,OtherBaseException (ConversionFailedException::error_code)
{
     Init (ConversionFailedException::error_code);
}

ConversionFailedException::ConversionFailedException (const char*msg ) :XdbBaseException (ConversionFailedException::error_code) ,OtherBaseException (ConversionFailedException::error_code)
{
     Init (ConversionFailedException::error_code, msg);
}

OpenFileFailedException::OpenFileFailedException (const char*msg ) :XdbBaseException (OpenFileFailedException::error_code) ,OtherBaseException (OpenFileFailedException::error_code)
{
	Init (OpenFileFailedException::error_code, msg);
}

OpenFileFailedException::OpenFileFailedException () :XdbBaseException (OpenFileFailedException::error_code) ,OtherBaseException (OpenFileFailedException::error_code)
{
	Init (OpenFileFailedException::error_code);
}

DocContentException::DocContentException (const char*msg ) :XdbBaseException (DocContentException::error_code) ,OtherBaseException (DocContentException::error_code)
{
    Init (DocContentException::error_code, msg);
}

DocContentException::DocContentException () :XdbBaseException (DocContentException::error_code) ,OtherBaseException (DocContentException::error_code)
{
    Init (DocContentException::error_code);
}
FileEmptyException::FileEmptyException (const char*msg ) :XdbBaseException (FileEmptyException::error_code) ,OtherBaseException (FileEmptyException::error_code)
{
	Init (FileEmptyException::error_code, msg);
}

FileEmptyException::FileEmptyException ( ) :XdbBaseException (FileEmptyException::error_code) ,OtherBaseException (FileEmptyException::error_code)
{
	Init (FileEmptyException::error_code);
}

FileTooLargeException::FileTooLargeException (const char*msg ) :XdbBaseException (FileTooLargeException::error_code) 
	,OtherBaseException (FileTooLargeException::error_code)
{
	Init (FileTooLargeException::error_code, msg);
}

FileTooLargeException::FileTooLargeException ( ) :XdbBaseException (FileTooLargeException::error_code) 
	,OtherBaseException (FileTooLargeException::error_code)
{
	Init (FileTooLargeException::error_code);
}

/**********************FullTextException Definition Start********************************/

//ResourceNotFoundExceptionFulltext
ResourceNotFoundExceptionFulltext::ResourceNotFoundExceptionFulltext() : XdbBaseException (ResourceNotFoundExceptionFulltext::error_code) ,FullTextException (ResourceNotFoundExceptionFulltext::error_code)
{
    Init (ResourceNotFoundExceptionFulltext::error_code);
}

ResourceNotFoundExceptionFulltext::ResourceNotFoundExceptionFulltext(const char* msg) : XdbBaseException (ResourceNotFoundExceptionFulltext::error_code) ,FullTextException (ResourceNotFoundExceptionFulltext::error_code)
{
    Init (ResourceNotFoundExceptionFulltext::error_code, msg);
}

//StopwordNotFoundException
StopwordNotFoundException::StopwordNotFoundException() : XdbBaseException (StopwordNotFoundException::error_code) ,FullTextException (StopwordNotFoundException::error_code)
{
    Init (StopwordNotFoundException::error_code);
}

StopwordNotFoundException::StopwordNotFoundException(const char* msg) : XdbBaseException (StopwordNotFoundException::error_code) ,FullTextException (StopwordNotFoundException::error_code)
{
    Init (StopwordNotFoundException::error_code, msg);
}

//FTWindowAndFTDistanceException
FTWindowAndFTDistanceException::FTWindowAndFTDistanceException() : XdbBaseException (FTWindowAndFTDistanceException::error_code) ,FullTextException (FTWindowAndFTDistanceException::error_code)
{
    Init (FTWindowAndFTDistanceException::error_code);
}

FTWindowAndFTDistanceException::FTWindowAndFTDistanceException(const char* msg) : XdbBaseException (FTWindowAndFTDistanceException::error_code) ,FullTextException (FTWindowAndFTDistanceException::error_code)
{
    Init (FTWindowAndFTDistanceException::error_code, msg);
}

//QueryContainsFtcontentException
QueryContainsFtcontentException::QueryContainsFtcontentException() : XdbBaseException (QueryContainsFtcontentException::error_code) ,FullTextException (QueryContainsFtcontentException::error_code)
{
    Init (QueryContainsFtcontentException::error_code);
}

QueryContainsFtcontentException::QueryContainsFtcontentException(const char* msg) : XdbBaseException (QueryContainsFtcontentException::error_code) ,FullTextException (QueryContainsFtcontentException::error_code)
{
    Init (QueryContainsFtcontentException::error_code, msg);
}

//FTLanguageOptionsException
FTLanguageOptionsException::FTLanguageOptionsException() : XdbBaseException (FTLanguageOptionsException::error_code) ,FullTextException (FTLanguageOptionsException::error_code)
{
    Init (FTLanguageOptionsException::error_code);
}

FTLanguageOptionsException::FTLanguageOptionsException(const char* msg) : XdbBaseException (FTLanguageOptionsException::error_code) ,FullTextException (FTLanguageOptionsException::error_code)
{
    Init (FTLanguageOptionsException::error_code, msg);
}

//FTCaseOptionException
FTCaseOptionException::FTCaseOptionException() : XdbBaseException (FTCaseOptionException::error_code) ,FullTextException (FTCaseOptionException::error_code)
{
    Init (FTCaseOptionException::error_code);
}

FTCaseOptionException::FTCaseOptionException(const char* msg) : XdbBaseException (FTCaseOptionException::error_code) ,FullTextException (FTCaseOptionException::error_code)
{
    Init (FTCaseOptionException::error_code, msg);
}

//WeightValueException
WeightValueException::WeightValueException() : XdbBaseException (WeightValueException::error_code) ,FullTextException (WeightValueException::error_code)
{
    Init (WeightValueException::error_code);
}

WeightValueException::WeightValueException(const char* msg) : XdbBaseException (WeightValueException::error_code) ,FullTextException (WeightValueException::error_code)
{
    Init (WeightValueException::error_code, msg);
}

//MildNotSelectionException
MildNotSelectionException::MildNotSelectionException() : XdbBaseException (MildNotSelectionException::error_code) ,FullTextException (MildNotSelectionException::error_code)
{
    Init (MildNotSelectionException::error_code);
}

MildNotSelectionException::MildNotSelectionException(const char* msg) : XdbBaseException (MildNotSelectionException::error_code) ,FullTextException (MildNotSelectionException::error_code)
{
    Init (MildNotSelectionException::error_code, msg);
}

//FTMatchOptionsExceptions
FTMatchOptionsExceptions::FTMatchOptionsExceptions() : XdbBaseException (FTMatchOptionsExceptions::error_code) ,FullTextException (FTMatchOptionsExceptions::error_code)
{
    Init (FTMatchOptionsExceptions::error_code);
}

FTMatchOptionsExceptions::FTMatchOptionsExceptions(const char* msg) : XdbBaseException (FTMatchOptionsExceptions::error_code) ,FullTextException (FTMatchOptionsExceptions::error_code)
{
    Init (FTMatchOptionsExceptions::error_code, msg);
}

//WildcardSyntaxException
WildcardSyntaxException::WildcardSyntaxException() : XdbBaseException (WildcardSyntaxException::error_code) ,FullTextException (WildcardSyntaxException::error_code)
{
    Init (WildcardSyntaxException::error_code);
}

WildcardSyntaxException::WildcardSyntaxException(const char* msg) : XdbBaseException (WildcardSyntaxException::error_code) ,FullTextException (WildcardSyntaxException::error_code)
{
    Init (WildcardSyntaxException::error_code, msg);
}

//IKAnalyzerException
IKAnalyzerException::IKAnalyzerException() : XdbBaseException (IKAnalyzerException::error_code) ,FullTextException (IKAnalyzerException::error_code)
{
    Init (IKAnalyzerException::error_code);
}

IKAnalyzerException::IKAnalyzerException(const char* msg) : XdbBaseException (IKAnalyzerException::error_code) ,FullTextException (IKAnalyzerException::error_code)
{
    Init (IKAnalyzerException::error_code, msg);
}

//FulltextResOperationException
FulltextResOperationException::FulltextResOperationException() : XdbBaseException (FulltextResOperationException::error_code) ,FullTextException (FulltextResOperationException::error_code)
{
    Init (FulltextResOperationException::error_code);
}

FulltextResOperationException::FulltextResOperationException(const char* msg) : XdbBaseException (FulltextResOperationException::error_code) ,FullTextException (FulltextResOperationException::error_code)
{
    Init (FulltextResOperationException::error_code, msg);
}

//UseDictionaryException
UseDictionaryException::UseDictionaryException() : XdbBaseException (UseDictionaryException::error_code) ,FullTextException (UseDictionaryException::error_code)
{
    Init (UseDictionaryException::error_code);
}

UseDictionaryException::UseDictionaryException(const char* msg) : XdbBaseException (UseDictionaryException::error_code) ,FullTextException (UseDictionaryException::error_code)
{
    Init (UseDictionaryException::error_code, msg);
}

/**********************FullTextException Definition End********************************/

BadKeyException::BadKeyException() : XdbBaseException (BadKeyException::error_code) ,OtherBaseException (BadKeyException::error_code)
{
    Init (BadKeyException::error_code);
}

BadKeyException::BadKeyException(const char* msg) : XdbBaseException (BadKeyException::error_code) ,OtherBaseException (BadKeyException::error_code)
{
    Init (BadKeyException::error_code, msg);
}

/******************kvtable exception definition begin*******************************/
KVTableException::KVTableException (const ErrorCode_t errcode) : XdbBaseException(errcode)
{
    Init (errcode) ;
}

KVTableException::KVTableException (const ErrorCode_t errcode, const char* msg) : XdbBaseException(errcode)
{
    Init (errcode, msg);
}
// kvtable exist begin
KVTableExistException::KVTableExistException() : XdbBaseException (KVTableExistException::error_code), KVTableException (KVTableExistException::error_code)
{
    Init (KVTableExistException::error_code);
}

KVTableExistException::KVTableExistException(const char* msg) : XdbBaseException (KVTableExistException::error_code), KVTableException (KVTableExistException::error_code)
{
    Init (KVTableExistException::error_code, msg);
}
// kvtable exist end

// kvtable not exist begin
KVTableNotExistException::KVTableNotExistException() : XdbBaseException (KVTableNotExistException::error_code), KVTableException (KVTableNotExistException::error_code)
{
    Init (KVTableNotExistException::error_code);
}

KVTableNotExistException::KVTableNotExistException(const char* msg) : XdbBaseException (KVTableNotExistException::error_code), KVTableException (KVTableNotExistException::error_code)
{
    Init (KVTableNotExistException::error_code, msg);
}
// kvtable not exist end

// kvindex table exist begin
KVIndexTableExistException::KVIndexTableExistException() : XdbBaseException (KVIndexTableExistException::error_code), KVTableException (KVIndexTableExistException::error_code)
{
    Init (KVIndexTableExistException::error_code);
}

KVIndexTableExistException::KVIndexTableExistException(const char* msg) : XdbBaseException (KVIndexTableExistException::error_code), KVTableException (KVIndexTableExistException::error_code)
{
    Init (KVIndexTableExistException::error_code, msg);
}
// kvindex table exist end

// kvindextable not exist begin
KVIndexTableNotExistException::KVIndexTableNotExistException() : XdbBaseException (KVIndexTableNotExistException::error_code), KVTableException (KVIndexTableNotExistException::error_code)
{
    Init (KVIndexTableNotExistException::error_code);
}

KVIndexTableNotExistException::KVIndexTableNotExistException(const char* msg) : XdbBaseException (KVIndexTableNotExistException::error_code), KVTableException (KVIndexTableNotExistException::error_code)
{
    Init (KVIndexTableNotExistException::error_code, msg);
}
// kvindextable not exist end

// kvtable dump failed begin
KVTableDumpFailedException::KVTableDumpFailedException() : XdbBaseException (KVTableDumpFailedException::error_code), KVTableException (KVTableDumpFailedException::error_code)
{
    Init (KVTableDumpFailedException::error_code);
}

KVTableDumpFailedException::KVTableDumpFailedException(const char* msg) : XdbBaseException (KVTableDumpFailedException::error_code), KVTableException (KVTableDumpFailedException::error_code)
{
    Init (KVTableDumpFailedException::error_code, msg);
}
// kvtable dump failed end

// kvtable load failed begin
KVTableLoadFailedException::KVTableLoadFailedException() : XdbBaseException (KVTableLoadFailedException::error_code), KVTableException (KVTableLoadFailedException::error_code)
{
    Init (KVTableLoadFailedException::error_code);
}

KVTableLoadFailedException::KVTableLoadFailedException(const char* msg) : XdbBaseException (KVTableLoadFailedException::error_code), KVTableException (KVTableLoadFailedException::error_code)
{
    Init (KVTableLoadFailedException::error_code, msg);
}
// kvtable load failed end

// kvtable index is unique now
KVIndexIsUniqueException::KVIndexIsUniqueException() : XdbBaseException (KVIndexIsUniqueException::error_code), KVTableException (KVIndexIsUniqueException::error_code)
{
	Init (KVIndexIsUniqueException::error_code);
}

KVIndexIsUniqueException::KVIndexIsUniqueException(const char* msg) : XdbBaseException (KVIndexIsUniqueException::error_code), KVTableException (KVIndexIsUniqueException::error_code)
{
	Init (KVIndexIsUniqueException::error_code, msg);
}
// kvtable index is unique now
/******************kvtable exception definition end*******************************/


/***********************************RollBackTransactionExceptin start********************************************/
const char* ServerAbortTxnException::GetErrorMessage (const ErrorCode_t error) 
{
    if ( ERR_BASE_POINT_LIMIT < error && error<= ERR_BASE_POINT_ROLLBACK ) { //RollBack Transaction error message array. 
        ErrorCode_t temp_code = abs(error - ERR_BASE_POINT_ROLLBACK) ;
        const char* err_ptr = *(Error_Message_RollBack + temp_code) ;
        return (XDBSTR(err_ptr));
    }else {
        return XDBSTR ("Unrecognized error information...\n"); 
    }
}

void ServerAbortTxnException::Init (const ErrorCode_t error)
{
     m_errorCode  = error;
     memset (m_infobuf, 0, sizeof(m_infobuf));
     const char* error_info = ServerAbortTxnException::GetErrorMessage (error); 
     uint32 length = ((uint32)strlen (error_info) > EXCEPTION_INFO_BUFLEN)? (EXCEPTION_INFO_BUFLEN -1): (uint32)strlen (error_info) ; 
     strncpy (m_infobuf, error_info, length);
}
void ServerAbortTxnException::Init (const ErrorCode_t error, const char *msg )
{
     m_errorCode  = error;
     memset (m_infobuf, 0, sizeof(m_infobuf));
     const char* error_info = ServerAbortTxnException::GetErrorMessage (error); 
     std::string str(error_info), str_la(msg);
     uint32 left_chars (EXCEPTION_INFO_BUFLEN), length (0);
     if (str_la.find(str) == std::string::npos ) {
         length = ((uint32)strlen (error_info) > EXCEPTION_INFO_BUFLEN )? (EXCEPTION_INFO_BUFLEN -1) : (uint32)strlen (error_info) ;
         strncpy (m_infobuf, error_info, length);
         left_chars = EXCEPTION_INFO_BUFLEN - length ;
     } 
     
     if (left_chars > 1 )
         snprintf (m_infobuf+length, left_chars-1,"|%s",msg);
}



ServerAbortTxnException::ServerAbortTxnException (const ErrorCode_t errcode) 
{    
    m_errorCode = errcode; memset(m_infobuf, 0, sizeof(m_infobuf)); 
}

ServerAbortTxnException::ServerAbortTxnException(const ErrorCode_t errcode, const char *msg)
{
    m_errorCode = errcode; strncpy(m_infobuf, msg, EXCEPTION_INFO_BUFLEN); 
}
//
//ServerAbortTxnException::ServerAbortTxnException(ServerAbortTxnException& obj)
//{
//    m_errorCode = obj.getErrorCode();
//    memset (m_infobuf,0, sizeof (obj.m_infobuf));
//    memcpy(m_infobuf, obj.m_infobuf, sizeof(m_infobuf));
//}
const ServerAbortTxnException& ServerAbortTxnException::operator= (const ServerAbortTxnException& obj)
{
    m_errorCode = obj.getErrorCode();
    memcpy(m_infobuf, obj.m_infobuf, sizeof(m_infobuf));
    return obj;
}

void ServerAbortTxnException::ThrowExceptionsByCode (const ErrorCode_t error) 
{
        switch (error){  
            case ERR_DUPLICATE_MODULE: {   //invalid db user exception.
                 throw DuplicateModuleException();
            } break;   
            case ERR_UPDATE_CONCURRENTLY :{
                throw UpdateConcurrentlyException ();
            } break; 
            default:{
                throw ServerAbortTxnException(error);
            } break;
        }            
}
void ServerAbortTxnException::ThrowExceptionsByCode (const ErrorCode_t error, const char* msg)
{
        switch (error){  
            case ERR_DUPLICATE_MODULE: {   //invalid db user exception.
                 throw DuplicateModuleException(msg);
            } break;          
            case ERR_UPDATE_CONCURRENTLY :{
                throw UpdateConcurrentlyException (msg);
            } break; 
            default:{
                 throw ServerAbortTxnException(error, msg);
            } break;
        }            
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


DuplicateModuleException::DuplicateModuleException (): ServerAbortTxnException (DuplicateModuleException::error_code)
{
    Init (DuplicateModuleException::error_code);
}

DuplicateModuleException::DuplicateModuleException (const char* msg): ServerAbortTxnException (DuplicateModuleException::error_code, msg) 
{
    Init (DuplicateModuleException::error_code,msg);
}


UpdateConcurrentlyException::UpdateConcurrentlyException (): ServerAbortTxnException(UpdateConcurrentlyException::error_code)
{
    Init (UpdateConcurrentlyException::error_code);
}

UpdateConcurrentlyException::UpdateConcurrentlyException (const char* msg): ServerAbortTxnException(UpdateConcurrentlyException::error_code)
{
    Init (UpdateConcurrentlyException::error_code,msg);
}

DeadLockException::DeadLockException (): ServerAbortTxnException(DeadLockException::error_code)
{
	Init (DeadLockException::error_code);
}

DeadLockException::DeadLockException (const char* msg): ServerAbortTxnException(DeadLockException::error_code)
{
	Init (DeadLockException::error_code,msg);
}

/***********************************RollBackTransactionExceptin start********************************************/


/******************GraphEngineException definition begin*******************************/
GraphEngineException::GraphEngineException (const ErrorCode_t errcode) : XdbBaseException(errcode)
{
    Init (errcode) ;
}

GraphEngineException::GraphEngineException (const ErrorCode_t errcode, const char* msg) : XdbBaseException(errcode)
{
    Init (errcode, msg);
}
// GraphOutOfMemoryException begin
GraphOutOfMemoryException::GraphOutOfMemoryException() : XdbBaseException (GraphOutOfMemoryException::error_code), GraphEngineException (GraphOutOfMemoryException::error_code)
{
    Init (GraphOutOfMemoryException::error_code);
}

GraphOutOfMemoryException::GraphOutOfMemoryException(const char* msg) : XdbBaseException (GraphOutOfMemoryException::error_code), GraphEngineException (GraphOutOfMemoryException::error_code)
{
    Init (GraphOutOfMemoryException::error_code, msg);
}
// GraphOutOfMemoryException end

GraphDropException::GraphDropException() : XdbBaseException(GraphDropException::error_code),GraphEngineException (GraphDropException::error_code)
{
	Init(GraphDropException::error_code);
}
GraphDropException::GraphDropException(const char* msg) : XdbBaseException(GraphDropException::error_code),GraphEngineException (GraphDropException::error_code)
{
	Init(GraphDropException::error_code,msg);
}

GraphModelNotExistException::GraphModelNotExistException() : XdbBaseException (GraphModelNotExistException::error_code), GraphEngineException (GraphModelNotExistException::error_code)
{
    Init (GraphModelNotExistException::error_code);
}

GraphModelNotExistException::GraphModelNotExistException(const char* msg) : XdbBaseException (GraphModelNotExistException::error_code), GraphEngineException (GraphModelNotExistException::error_code)
{
    Init (GraphModelNotExistException::error_code, msg);
}

GraphCurrentGraphNotExistException::GraphCurrentGraphNotExistException() : XdbBaseException (GraphCurrentGraphNotExistException::error_code), GraphEngineException (GraphCurrentGraphNotExistException::error_code)
{
    Init (GraphCurrentGraphNotExistException::error_code);
}

GraphCurrentGraphNotExistException::GraphCurrentGraphNotExistException(const char* msg) : XdbBaseException (GraphCurrentGraphNotExistException::error_code), GraphEngineException (GraphCurrentGraphNotExistException::error_code)
{
    Init (GraphCurrentGraphNotExistException::error_code, msg);
}

GraphParseFileNotExistException::GraphParseFileNotExistException() : XdbBaseException (GraphParseFileNotExistException::error_code), GraphEngineException (GraphParseFileNotExistException::error_code)
{
    Init (GraphParseFileNotExistException::error_code);
}

GraphParseFileNotExistException::GraphParseFileNotExistException(const char* msg) : XdbBaseException (GraphParseFileNotExistException::error_code), GraphEngineException (GraphParseFileNotExistException::error_code)
{
    Init (GraphParseFileNotExistException::error_code, msg);
}

GraphAddTripleException::GraphAddTripleException() : XdbBaseException (GraphAddTripleException::error_code), GraphEngineException (GraphAddTripleException::error_code)
{
    Init (GraphAddTripleException::error_code);
}

GraphAddTripleException::GraphAddTripleException(const char* msg) : XdbBaseException (GraphAddTripleException::error_code), GraphEngineException (GraphAddTripleException::error_code)
{
    Init (GraphAddTripleException::error_code, msg);
}

GraphDeleteTripleException::GraphDeleteTripleException() : XdbBaseException (GraphDeleteTripleException::error_code), GraphEngineException (GraphDeleteTripleException::error_code)
{
    Init (GraphDeleteTripleException::error_code);
}

GraphDeleteTripleException::GraphDeleteTripleException(const char* msg) : XdbBaseException (GraphDeleteTripleException::error_code), GraphEngineException (GraphDeleteTripleException::error_code)
{
    Init (GraphDeleteTripleException::error_code, msg);
}

GraphUpdateTripleException::GraphUpdateTripleException() : XdbBaseException (GraphUpdateTripleException::error_code), GraphEngineException (GraphUpdateTripleException::error_code)
{
    Init (GraphUpdateTripleException::error_code);
}

GraphUpdateTripleException::GraphUpdateTripleException(const char* msg) : XdbBaseException (GraphUpdateTripleException::error_code), GraphEngineException (GraphUpdateTripleException::error_code)
{
    Init (GraphUpdateTripleException::error_code, msg);
}

GraphSearchTripleException::GraphSearchTripleException() : XdbBaseException (GraphSearchTripleException::error_code), GraphEngineException (GraphSearchTripleException::error_code)
{
    Init (GraphSearchTripleException::error_code);
}

GraphSearchTripleException::GraphSearchTripleException(const char* msg) : XdbBaseException (GraphSearchTripleException::error_code), GraphEngineException (GraphSearchTripleException::error_code)
{
    Init (GraphSearchTripleException::error_code, msg);
}

GraphResultTypeException::GraphResultTypeException() : XdbBaseException (GraphResultTypeException::error_code), GraphEngineException (GraphResultTypeException::error_code)
{
    Init (GraphResultTypeException::error_code);
}

GraphResultTypeException::GraphResultTypeException(const char* msg) : XdbBaseException (GraphResultTypeException::error_code), GraphEngineException (GraphResultTypeException::error_code)
{
    Init (GraphResultTypeException::error_code, msg);
}

GraphUniversalException::GraphUniversalException() : XdbBaseException (GraphUniversalException::error_code), GraphEngineException (GraphUniversalException::error_code)
{
    Init (GraphUniversalException::error_code);
}

GraphUniversalException::GraphUniversalException(const char* msg) : XdbBaseException (GraphUniversalException::error_code), GraphEngineException (GraphUniversalException::error_code)
{
    Init (GraphUniversalException::error_code, msg);
}

QueryInterruptedException::QueryInterruptedException() : XdbBaseException (QueryInterruptedException::error_code), QueryEngineException (QueryInterruptedException::error_code)
{
	Init (QueryInterruptedException::error_code);
}

QueryInterruptedException::QueryInterruptedException(const char* msg) : XdbBaseException (QueryInterruptedException::error_code), QueryEngineException (QueryInterruptedException::error_code)
{
	Init (QueryInterruptedException::error_code, msg);
}

QueryTimeoutException::QueryTimeoutException() : XdbBaseException (QueryTimeoutException::error_code), QueryEngineException (QueryTimeoutException::error_code)
{
	Init (QueryTimeoutException::error_code);
}

QueryTimeoutException::QueryTimeoutException(const char* msg) : XdbBaseException (QueryTimeoutException::error_code), QueryEngineException (QueryTimeoutException::error_code)
{
	Init (QueryTimeoutException::error_code, msg);
}


}  // namespace EXCEPTION
}  // namespace FounderXDB
