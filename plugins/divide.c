int execute(int len, int values[]){
	int result = values[0];
	for (int i = 1; i < len; i++)
		result /= values[i];
   return result;
} 
