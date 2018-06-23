char	*vts_server = "localhost";
int		vts_port      = 8888;
int		vts_rc;
int		vts_column_size;
char	vts_column_name[] = "v_Composite";
char	vts_column_name_eval[20];
PVCI2	pvci = 0;

/******************************************************************************
 * VTS Connection
 *****************************************************************************/
int VTS_Connect()
{
	pvci = lrvtc_connect(vts_server, vts_port, VTOPT_KEEP_ALIVE);
	if (pvci == VTCERR_OK)
	{
		lr_log_message("***[VTS]*** Conneced to VTS Server: %s, Call to open returned: %d\n", vts_server, pvci);
	} else
	{
		lr_log_message("***[VTS]*** VTS Connection attemp failed with error code %d\n", pvci);
		lr_exit(LR_EXIT_VUSER, LR_FAIL);
	}
	
	return 0;
}

/******************************************************************************
 * VTS Disconnection
 *****************************************************************************/
int VTS_Disconnect()
{
	int rc;
	
	rc = lrvtc_disconnect();
	lr_log_message("***[VTS]*** Disconnection result rc=%d\n", rc);
	
	return rc;
}

/******************************************************************************
 * VTS Get Record Count
 *****************************************************************************/
int VTS_GetRecordCount(char *vts_column_name)
{
	int column_size;
	
	// The number of fields containing data in the column.
	column_size = lrvtc_column_size(vts_column_name);  
	if (!column_size)
	{
		lr_log_message("***[VTS]*** VTS column %s is empty or not found, colsize = %d.\n", vts_column_name, column_size);
	}
	
	return column_size;
}
/******************************************************************************
 * VTS Get Record
 *****************************************************************************/
int VTS_GetRecord(char *vts_column_name)
{
	int rc;
	
	// Returns zero on success or one of the Error Codes.
	rc = lrvtc_retrieve_message(vts_column_name); 
	
	// 컬럼명을 {컬럼명} 형태로 저장해 놓는다.
	sprintf(vts_column_name_eval, "{%s}", vts_column_name);
	return rc;
}

/******************************************************************************
 * VTS Put Record
 *****************************************************************************/
int VTS_PutRecord(char *vts_column_name, char *columnValue)
{
	int rc;
	
	// Returns zero on success or one of the Error Codes.
	rc = lrvtc_send_message(vts_column_name, columnValue); 
	lr_log_message("***[VTS]*** Disconnect result rc=%d\n", rc);
	
	return rc;
}

/******************************************************************************
 * CHANGE HERE - Get Composite Data to Token
 *****************************************************************************/
int GetCompositeData(char *columnValue)
{
	char separators[] = ":";
	char *token;
	
	// 1. current Date
	token = (char *)strtok(columnValue, separators);
	if (!token)
	{
		lr_log_message("***[VTS]*** Composite record is invailid!");
		return -1;
	}
	lr_save_string(token, "v_currDate");
	
	// 2. current Time
	token = (char *)strtok(NULL, separators);
	lr_save_string(token, "v_currTime");
	
	// 3. random Number
	token = (char *)strtok(NULL, separators);
	lr_save_string(token, "v_rnd");
	
	return 0;
}