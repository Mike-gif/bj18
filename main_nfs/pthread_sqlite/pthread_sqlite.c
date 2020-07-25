#include "pthread_sqlite.h"	
#include "sqlite3.h"

#define TEMPERATURE_MAX   1
#define TEMPERATURE_MIN   2
#define HUMIDITY_MAX      3
#define HUMIDITY_MIN      4
#define ILLUMINATION_MAX  5
#define ILLUMINATION_MIN  6
#define TELEPHONE_NUM     7
#define MODE              8
#define IFNOCICE          9
#define INTERVAL          10 
#define UPDATE_PERIOD     11
#define USER_NAME         12
#define PASSWORD          13

int InsertOrAddFlag = 0;
float callback_val[15];
char callback_str[20];

extern slinklist slinkHead, slinkTail;
/*
**创建数据库表
*/
int Create_table (void)
{
    char sql[1024];
    sqlite3 *db;
    char *err_msg = 0;
    int recode;

    recode = sqlite3_open(SQLITE_OPEN, &db);
    if( recode != SQLITE_OK)
    {
        printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
        printf("open OK!\n");
#endif  

        sprintf(sql, "create table env(dev_no int not null,\
                                       temperatureMax float not null,\
                                       temperatureMin float not null,\
                                       humidityMax float not null,\
                                       humidityMin float not null,\
                                       illuminationMax float not null,\
                                       illuminationMin float not null,\
                                       telephoneNum varchar(11) not null,\
                                       mode int not null,\
                                       ifnocice int not null,\
                                       interval int not null,\
                                       updatePeriod int not null,\
                                       username varchar(20) not null,\
                                       password int not null);");
        recode = sqlite3_exec(db, sql,0,0,&err_msg);
        if(recode != SQLITE_OK)
        {
            if(strcmp(err_msg,"table env already exists"))
            {
                printf("Error:%s", err_msg);
                sqlite3_close(db);
                return 1;
            }
            else
            {
                printf("table env already exist, so open it ok!\n");
            }
            
        }
        else
        {
#if DEBUG_SQLITE
             Init_table_env();
             printf("creat env OK!\n");
#endif
        }
        sprintf(sql, "create table collect_env(dev_no32 int not null,\
                                              temperatureVal float not null,\
                                              humifityVal float not null,\
                                              illuminationVal float not null,\
                                              envtime varchar(10) not null);");
        recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if(recode != SQLITE_OK)
        {
            if(strcmp(err_msg,"table collect_env already exists"))
            {
                printf("Error:%s", err_msg);
                sqlite3_close(db);
                return 1;
            }
            else
            {
                printf("table collect_env already exists, so open it ok\n");
            }
            
        }
#if DEBUG_SQLITE
           printf(" creat collect_env OK!\n");
#endif

#if DEBUG_SQLITE
           printf("collect_env init OK!\n");
#endif
         sprintf(sql, "create table goods (dev_no int not null,\
                                           goodsId int not null,\
                                           goodsCount int not null,\
                                           goodsTime varchar(10) not null);");
                                          
         recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
         if(recode != SQLITE_OK)
         {
             if(strcmp(err_msg,"table goods already exists"))
             {
                 printf("Error:%s", err_msg);
                 sqlite3_close(db);
                 return 1;
             }
             else
             {
                 printf("table goods already exists, so open it ok");
             }
             
         }
#if DEBUG_SQLITE
           printf("creat goods OK!\n");
#endif
           sqlite3_close(db);
           return 0;
    }
   

}

/*
**初始化环境表，
*/
int Init_table_env (void)
{
    char sql[1024];
    sqlite3 *db;
    char *err_msg = 0;
    int recode;
    int i;

    for(i = 1; i < 2; i++)
    {
        sprintf(sql, "insert into env values('%d', 25.4, 10.5, 55.8, 10.8,\
                                             258, 129, '12345678901', 1,\
                                             2, 3, 4, 'binea', 123456);", i);
        recode = sqlite3_open(SQLITE_OPEN, &db);
        if(recode != SQLITE_OK)
        {
            printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }
        else
        {
#if DEBUG_SQLITE
            printf("open OK!\n");
#endif
            recode = sqlite3_exec(db, sql,0, 0, &err_msg);
            if(recode != SQLITE_OK)
            {
                printf("Error:%s", err_msg);
                sqlite3_close(db);
                return 1;
            }
            else
            {
#if DEBUG_SQLITE
                printf("insert env OK!\n");
#endif
                sqlite3_close(db);
            }
            
        }
        
    }
    return 0;
}

/*
**回调函数，查询之后的信息都通过 //胡磊2020.07.19：猜测：将数据库查询结果保存进数组callback_val[15]
*/
static int callback_getenv (void * para, int n_column, char ** column_value, char ** column_name )
{
   int i;
#if DEBUG_SQLITE
   printf("n_column = %d\n", n_column);
#endif
    for(i = 0; i < n_column; i++)
     {
         if(n_column - 1 == i)
         {
            strcpy(callback_str, column_value[i]);
#if DEBUG_SQLITE 
            printf("%s = %s\n", column_name[i], callback_str);
#endif
         }
         else
         {
             callback_val[i] = (float)atof(column_value[i]);
#if DEBUG_SQLITE 
             printf("%s = %f\n", column_name[i], callback_val[i]);
#endif
         }        
     } 
     InsertOrAddFlag = (int)callback_val[1];
#if DEBUG_SQLITE
    printf("************\n");
#endif
    return 0;  

}

static int callback_getenv_s(void *para, int n_column, char** column_value, char** column_name)
{
    strcpy(callback_str, column_value[0]);
#if DEBUG_SQLITE
   printf("%s = %s\n", column_name[0], callback_str);
#endif
   return 0;
}

/*
**更新env参数的SQLite指令选择函数
**这个函数中只能插入int类型的值
*/
//胡磊2020.07.17：参数说明：val：值，dev_no_t：设备号， no：决定采哪个类型的值，如最高温度、最大湿度。
int setEnv (float val, int dev_no_t, int no)
{
    char sql[1024];
#if DEBUG_SQLITE
	printf("In pthread_sqlite setEnv no = %d, val = %f, recive_phone = %s\n", no, val, recive_phone);
#endif
   switch (no)
	{
	case 1:
		sprintf (sql, "update env set temperatureMax = '%f' where dev_no = '%d';", val, dev_no_t); //最高温度
		break;
	case 2:
		sprintf (sql, "update env set temperatureMin = '%f' where dev_no = '%d';", val, dev_no_t); //最低温度
		break;
	case 3:
		sprintf (sql, "update env set humidityMax = '%f' where dev_no = '%d';", val, dev_no_t); //最高湿度
		break;
	case 4:
		sprintf (sql, "update env set humidityMin = '%f' where dev_no = '%d';", val, dev_no_t); //最低湿度
		break;
	case 5:
		sprintf (sql, "update env set illuminationMax = '%f' where dev_no = '%d';", val, dev_no_t); //最高光照
		break;
	case 6:
		sprintf (sql, "update env set illuminationMin = '%f' where dev_no = '%d';", val, dev_no_t); //最低光照
		break;
    }

    sqlite3 *db;
    char *err_msg = 0;
    int recode;
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
		printf ("open OK!\n");
#endif  
       recode=sqlite3_exec(db, sql, 0, 0, &err_msg);
       if(recode != SQLITE_OK) 
       {
           printf("Error:%s", err_msg);
           sqlite3_close(db);
           return 1;
       }  
       else
       {
#if DEBUG_SQLITE
			printf ("Insert OK!\n");
#endif
			sqlite3_close (db);
			return 0;          
       }
          
    }
    

}


/*
**更新env参数的SQLite指令选择函数
**这个函数中只能插入字符串类型的值
*/
int setEnv_s (char *val, int no)
{
    char sql[1024];
#if DEBUG_SQLITE
	printf("In setEnv_s no = %d, val = %s\n", no, val);
#endif
    switch(no)
	{
	case 7:
		sprintf (sql, "update env set telephoneNum = '%s';", val);
		break;
	case 12:
		sprintf (sql, "update env set username = '%s';", val);
		break;
	}

    sqlite3 *db;
    char *err_msg = 0;
    int recode;
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
        printf("Can't Open Database: %s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
		printf ("open OK!\n");
#endif
    recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if(recode != SQLITE_OK)
    {
        printf("Error: %s", err_msg);
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
           printf("Insert OK!\n");
#endif
           sqlite3_close(db);
           return 0;
    }
    
    }
    
}


/*
**获得env参数的SQLite指令选择函数
*/
int getEnv_select(int storageNum_t, int no)
{
    char sql[1024];
    sqlite3 *db;
    char *err_msg = 0;
    int recode;

    switch(no)
    {
      case 1:
          sprintf(sql, "select temperatureMax from env where dev_no '%d';", storageNum_t);
          break;
      case 2:
          sprintf(sql, "select temperatureMin from env where dev_no '%d';", storageNum_t);
          break;
      case 3:
          sprintf(sql, "select humidityMax from env where dev_no  = '%d';", storageNum_t);
          break;
      case 4:
          sprintf(sql, "select humidityMin from env where dev_no = '%d'", storageNum_t);
          break;
      case 5:
          sprintf(sql, "select illuminationMax from env where dev_no = '%d';", storageNum_t);
          break;
      case 6:
          sprintf(sql, "select illuminationMin from env where dev_no ='%d'", storageNum_t);
          break;     
    }
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
        printf("Can't open database:%s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
          printf("sqlite OK!\n");
#endif
        if((7 == no) || (12 == no))
        {
            recode = sqlite3_exec(db, sql, callback_getenv_s, 0, &err_msg);
        }
        else
        {
            recode = sqlite3_exec(db, sql, callback_getenv, 0, &err_msg);
        }
        if(recode != SQLITE_OK)
        {
            printf("Error:%s", err_msg);
            sqlite3_close(db);
            return 1;
        }
        else
		{
#if DEBUG_SQLITE
			printf ("get env OK!\n");
#endif
			sqlite3_close (db);
			return 0;
		}
    }
    
}


/*
**更新env参数
*/
int updateEnv (struct env_info_clien_addr env_info_clien_addr_t, int storageNum_t)
{
   setEnv(env_info_clien_addr_t.storage_no[storageNum_t].temperatureMAX, storageNum_t,TEMPERATURE_MAX);
   setEnv (env_info_clien_addr_t.storage_no[storageNum_t].temperatureMIN, storageNum_t, TEMPERATURE_MIN);
   setEnv (env_info_clien_addr_t.storage_no[storageNum_t].humidityMAX, storageNum_t, HUMIDITY_MAX);
   setEnv (env_info_clien_addr_t.storage_no[storageNum_t].humidityMIN, storageNum_t, HUMIDITY_MIN);
   setEnv (env_info_clien_addr_t.storage_no[storageNum_t].illuminationMAX, storageNum_t, ILLUMINATION_MAX);
   setEnv (env_info_clien_addr_t.storage_no[storageNum_t].illuminationMIN, storageNum_t, ILLUMINATION_MIN);
   	if (strcmp(recive_phone, "0") != 0 && strcmp(center_phone, "0") != 0){
	setEnv_s (recive_phone, TELEPHONE_NUM);
	setEnv_s (center_phone, USER_NAME);
	}

    return 0;
}

/*
**获得env参数 //胡磊2020.07.19：将数据库执行查询的结果保存进env_info_clien_add类型结构体env_info_clien_addr_t的对应变量和 all_info_RT的对应变量
*/
int getEnv (struct env_info_clien_addr *env_info_clien_addr_t, int storageNum_t)
{
   getEnv_select(storageNum_t, TEMPERATURE_MAX);
   env_info_clien_addr_t->storage_no[storageNum_t].temperatureMAX = callback_val[0]; //胡磊2020.07.19：每执行一次getEnv就会产生一个新的callback_val[0]，所以每次都是赋值callback_val[0]。
   all_info_RT.storage_no[storageNum_t].temperatureMAX = callback_val[0];
   getEnv_select(storageNum_t, TEMPERATURE_MIN);
   env_info_clien_addr_t->storage_no[storageNum_t].temperatureMIN = callback_val[0];
   all_info_RT.storage_no[storageNum_t].temperatureMIN = callback_val[0];
   getEnv_select(storageNum_t, HUMIDITY_MAX);
   env_info_clien_addr_t->storage_no[storageNum_t].humidityMAX = callback_val[0];
   all_info_RT.storage_no[storageNum_t].humidityMAX = callback_val[0];
   getEnv_select(storageNum_t, HUMIDITY_MIN);
   env_info_clien_addr_t->storage_no[storageNum_t].humidityMIN = callback_val[0];
   all_info_RT.storage_no[storageNum_t].humidityMIN = callback_val[0];
   getEnv_select(storageNum_t, ILLUMINATION_MAX);
   env_info_clien_addr_t->storage_no[storageNum_t].illuminationMAX = callback_val[0];
   all_info_RT.storage_no[storageNum_t].illuminationMAX = callback_val[0];
   getEnv_select(storageNum_t, ILLUMINATION_MIN);
   env_info_clien_addr_t->storage_no[storageNum_t].illuminationMIN = callback_val[0];
   all_info_RT.storage_no[storageNum_t].illuminationMIN = callback_val[0];

   return 0;

}

int insertCollect_env (struct env_info_clien_addr env_info_clien_addr_t, int storage_num)
{
#if DEBUG_SQLITE
     printf("insertCollect_env ok\n");
#endif
#if 1
    sqlite3 *db;
    char *err_msg = 0;
    int recode;
    char sql[1024];
    sprintf(sql, "insert into collect_env values(%d, %f, %f, %f, '%s');",
            storage_num,
            env_info_clien_addr_t.storage_no[storage_num].temperature,
            env_info_clien_addr_t.storage_no[storage_num].humidity,
            env_info_clien_addr_t.storage_no[storage_num].illumination,
            env_info_clien_addr_t.storage_no[storage_num].samplingTime
            );
     recode = sqlite3_open(SQLITE_OPEN, &db);
     if(recode != SQLITE_OK)
     {
         printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
         sqlite3_close(db);
         return 1;
     }
     else
     {
#if DEBUG_SQLITE
         printf("Open OK!\n");
#endif
         recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
         if(recode != SQLITE_OK)
         {
             printf("Error:%s", err_msg);
             sqlite3_close(db);
             return 1;
         }
         else
         {
#if DEBUG_SQLITE
            printf("Insert collect_env OK!\n");
#endif
            sqlite3_close(db);
            return 0;
         }
         
     }
#endif    
}

int getCollect_env (char itime_t[], int storage_num_t)
{
#if DEBUG_SQLITE
     printf("getCollect_env OK!\n");
#endif
    sqlite3 *db;
    char *err_msg = 0;
    int recode;
    recode = sqlite3_open(SQLITE_OPEN, &db);
    char sql[1024];
    sprintf(sql, "select * from collect_env where envtime = '%s' and dev_no = %d;", itime_t, storage_num_t);
    if( recode != SQLITE_OK)
    {
        printf("Can't Open Database:%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
        printf("open OK!\n");
#endif
    recode = sqlite3_exec(db, sql, callback_getenv, 0, &err_msg);
       if(recode != SQLITE_OK)
       {
          printf("Error:%s", err_msg);
          sqlite3_close(db);
          return 1;
       }
       else
       {
#if DEBUG_SQLITE
           printf("select collect_env OK!\n");
#endif
          sqlite3_close(db);
       }
       env_info_clien_addr_s.storage_no[storage_num_t].temperature = callback_val[1];
       env_info_clien_addr_s.storage_no[storage_num_t].humidity = callback_val[2];
       env_info_clien_addr_s.storage_no[storage_num_t].illumination = callback_val[3];
       strcpy(env_info_clien_addr_s.storage_no[storage_num_t].samplingTime, callback_str);

       all_info_RT.storage_no[storage_num_t].temperature = callback_val[1];
       all_info_RT.storage_no[storage_num_t].humidity = callback_val[2];
       all_info_RT.storage_no[storage_num_t].illumination = callback_val[3];
       strcpy(all_info_RT.storage_no[storage_num_t].samplingTime, callback_str);
#if 1
      printf ("dev_no:%d  temperature = %0.1f\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].temperature);
		printf ("dev_no:%d  humidity = %0.1f\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].humidity);
		printf ("dev_no:%d  illumination = %0.1f\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].illumination);
		printf ("dev_no:%d  time = %s\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].samplingTime);
		printf ("-----------------------------\n");
#endif  
    }
    return 0;
}

int getCollect_Current_env (int storage_num_t)
{
#if DEBUG_SQLITE
   printf ("getCollect_env OK!\n");
#endif
   sqlite3 *db;
   char *err_msg = 0;
   int recode;
   recode = sqlite3_open(SQLITE_OPEN, &db);
   char sql[1024];
   sprintf(sql, "select * from collect_env where dev_no = %d order by envtime desc limit 1", storage_num_t);
   
   if(recode != SQLITE_OK)
   {
       printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
       sqlite3_close(db);
       return 1;
   }
   else
   {
#if DEBUG_SQLITE
      printf("open OK!\n");
#endif
      recode = sqlite3_exec(db, sql, callback_getenv, 0, &err_msg);
      if(recode != SQLITE_OK)
      {
          printf("Error:%s", err_msg);
          sqlite3_close(db);
          return 1;
      }
      else
      {
#if DEBUG_SQLITE
          printf("select collect_env OK!\n");
#endif
          sqlite3_close(db);
      }
      env_info_clien_addr_s.storage_no[storage_num_t].temperature = callback_val[1];
      env_info_clien_addr_s.storage_no[storage_num_t].humidity = callback_val[2];
      env_info_clien_addr_s.storage_no[storage_num_t].illumination = callback_val[3];
      strcpy(env_info_clien_addr_s.storage_no[storage_num_t].samplingTime, callback_str);


      all_info_RT.storage_no[storage_num_t].temperature = callback_val[1];
      all_info_RT.storage_no[storage_num_t].humidity = callback_val[2];
      all_info_RT.storage_no[storage_num_t].illumination = callback_val[3];
      strcpy(all_info_RT.storage_no[storage_num_t].samplingTime, callback_str);

#if 1
		printf ("dev_no:%d  temperature = %0.1f\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].temperature);
		printf ("dev_no:%d  humidity = %0.1f\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].humidity);
		printf ("dev_no:%d  illumination = %0.1f\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].illumination);
		printf ("dev_no:%d  time = %s\n", storage_num_t, env_info_clien_addr_s.storage_no[storage_num_t].samplingTime);
		printf ("-----------------------------\n");
#endif
   }
   return 0;
}

int viewGoods (int storageNum_t, int goodsKinds_t)
{
#if DEBUG_SQLITE
   printf("viewGoods OK!\n");
#endif
   sqlite3 *db;
   char *err_msg = 0;
   int recode;
   recode = sqlite3_open(SQLITE_OPEN, &db);
   char sql[1024];
   sprintf(sql, "select * from goods where dev_no = '%d' and goodsId = '%d';", storageNum_t, goodsKinds_t); 

   if(recode != SQLITE_OK)
   {
       printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
       sqlite3_close(db);
       return 1;
   }
   else
   {
#if DEBUG_SQLITE
      printf("open OK!\n");
#endif
    InsertOrAddFlag = 0;
    recode =sqlite3_exec(db, sql, callback_getenv, 0, &err_msg);
    if(recode != SQLITE_OK)
    {
        printf("Error:%s", err_msg);
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
         printf("view goods OK!\n");
#endif 
       sqlite3_close(db);
    }
    
   }
   return InsertOrAddFlag;
}

/*胡磊2020.07.16：入库新的货物,入库货物种类仓库中没有 */
int insertGoods (struct env_info_clien_addr env_info_clien_addr_t, int storageNum_t, int goodsKinds_t)
{
#if DEBUG_SQLITE
    printf("insertGoods ok\n");
    printf("storageNum = %d, goodsKinds = %d\n", storageNum_t, goodsKinds_t);
#endif
    sqlite3 *db;
    char *err_msg = 0;
    int recode;
    char sql[1024];
    sprintf(sql, "insert into goods values(%d,%d,%d,'%s',);",
            storageNum_t, 
            env_info_clien_addr_t.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type,
            env_info_clien_addr_t.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count,
            env_info_clien_addr_t.storage_no[storageNum_t].samplingTime
    );
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
         printf("Can't Open Database:%s\n", sqlite3_errmsg(db));
         sqlite3_close(db);
         return 1;
    }
    else
    {
#if DEBUG_SQLITE
        printf("open OK!\n");
#endif
        recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if(recode != SQLITE_OK)
        {
            printf("Error:%s", err_msg);
            sqlite3_close(db);
            return 1;
        }
        else
        {
#if DEBUG_SQLITE
             printf("Insert goods OK!\n");
#endif
            sqlite3_close(db);
        }
        
    }
    return 0;

}
/*胡磊2020.07.16：删除该货号*/
int deleteGoods (int storageNum_t, int goodsKinds_t)
{
#if DEBUG_SQLITE
      printf("deleteGoods OK\n");
#endif
    sqlite3 *db;
    char *err_msg = 0;
    int recode;
    recode = sqlite3_open(SQLITE_OPEN, &db);
    char sql[1024];

    sprintf(sql, "delete from goods where dev_no = '%d' and goodsId = '%d';", storageNum_t, goodsKinds_t);

    if(recode != SQLITE_OK)
    {
        printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
       printf("open OK!\n");
#endif
       recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
       if(recode != SQLITE_OK)
       {
           printf("Error:%s", err_msg);
           sqlite3_close(db);
           return 1;
       }
       else
       {
#if DEBUG_SQLITE
            printf("delete goods OK!\n");
#endif
            sqlite3_close(db);
       }       
    }
     return 0;
}
/*胡磊2020.07.16：增加货物，入库货物种类仓库中没有*/
int addGoods (struct env_info_clien_addr env_info_clien_addr_t, int storageNum_t, int goodsKinds_t)
{
#if DEBUG_SQLITE
     printf("addGoods ok\n");
     printf("storageNum = %d, goodsKinds = %d\n", storageNum_t, goodsKinds_t);
#endif
    sqlite3 *db;
    char *err_msg = 0;
    int recode, addNum = 0;
    char sql[1024];
    addNum = env_info_clien_addr_t.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count;
    sprintf(sql, "select * from goods where dev_no = '%d' and goodsId = '%d';", storageNum_t, goodsKinds_t);
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
        printf("Can't open database:%s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
       printf("open OK!\n");
#endif
       recode = sqlite3_exec(db, sql, callback_getenv, 0, &err_msg);
       if(recode != SQLITE_OK)
       {
           printf("Error:%s", err_msg);
           sqlite3_close(db);
           return 1;
       }
       else
       {
#if DEBUG_SQLITE
           printf("add select goods OK!\n");
#endif
           sqlite3_close(db);
       }
       env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type = (int)callback_val[1];
       env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count = (int)callback_val[2];
    }
    sprintf(sql, "update goods set goodsCount = '%d', goodsTime = '%s' where dev_no = '%d'and goodsId = '%d';", 
            env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count + addNum,  //胡磊2020.07.19：为什么要加addNum？重复计算？
            env_info_clien_addr_s.storage_no[storageNum_t].samplingTime,
			storageNum_t,
		    env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type);
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
        printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
       printf("open OK!\n");
#endif
      recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
      if(recode != SQLITE_OK)
      {
          printf("Error:%s", err_msg);
          sqlite3_close(db);
          return 1;
      }
      else
      {
#if DEBUG_SQLITE
           printf("add goods OK!\n");
#endif
       printf ("the sto num is %d\n the goodstype is %d\n the goodscount is%d\n",  storageNum_t , env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type, env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count + addNum);
			sqlite3_close (db);
      }
    }
    
     return 0;    
}

int reduceGoods (struct env_info_clien_addr env_info_clien_addr_t, int storageNum_t, int goodsKinds_t)
{
#if DEBUG_SQLITE
    printf("reduceGoods ok\n");
    printf("storageNum = %d, goodsKinds = %d\n", storageNum_t, goodsKinds_t);
#endif
    sqlite3 *db;
    char *err_msg = 0;
    int recode, addNum = 0;
    char sql[1024];
    addNum = env_info_clien_addr_t.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count;

    sprintf(sql, "select * from goods where dev_no = '%d' and goodsId = '%d';", storageNum_t, goodsKinds_t);
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
        printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
        printf("open OK!\n");
#endif
      recode = sqlite3_exec(db, sql, callback_getenv, 0, &err_msg);
      if(recode != SQLITE_OK)
      {
          printf("Error：%s", err_msg);
          sqlite3_close(db);
          return 1;
      }
      else
      {
#if DEBUG_SQLITE
           printf("Reduce select goods OK!\n");
#endif
           sqlite3_close(db);
      }
      env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type = (int)callback_val[1];
      env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count = (int)callback_val[2];
    }
    sprintf(sql, "update goods set goodsCount = '%d', goodsTime = '%s' where dev_no = '%d' and goodsId = '%d';", 
            (env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count - addNum),
            env_info_clien_addr_s.storage_no[storageNum_t].samplingTime, 
            storageNum_t, 
            env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type);
    recode = sqlite3_open(SQLITE_OPEN, &db);
    if(recode != SQLITE_OK)
    {
        printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    else
    {
#if DEBUG_SQLITE
            printf("open OK!\n");
#endif
        recode = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if(recode != SQLITE_OK)
        {
            printf("Error:%s", err_msg);
            sqlite3_close(db);
            return 1;
        }
        else
        {
#if DEBUG_SQLITE
             printf("reduce goods OK!\n");
#endif
             sqlite3_close(db);
        }        
    }
    if(0 == viewGoods(storageNum_t, goodsKinds_t))
    {
        deleteGoods (storageNum, goodsKinds);
    }
    
    return 0;
}
int getGoods (int storageNum_t, int goodsKinds_t)
{
#if DEBUG_SQLITE
      printf("getGoods ok\n");
#endif
      sqlite3 *db;
      char *err_msg = 0;
      int recode;
      recode = sqlite3_open(SQLITE_OPEN, &db);
      char sql[1024];

      sprintf(sql, "select * from goods where dev_no = '%d' and goodsId = '%d';", storageNum_t, goodsKinds_t);
      if(recode != SQLITE_OK)
      {
          printf("Can't Open Database:%s!\n", sqlite3_errmsg(db));
          sqlite3_close(db);
          return 1;
      }
      else
      {
#if  DEBUG_SQLITE
            printf("open OK!\n");
#endif  
            recode = sqlite3_exec(db, sql, callback_getenv, 0, &err_msg);
            if(recode != SQLITE_OK)
            {
                printf("Error: %s", err_msg);
                sqlite3_close(db);
                return 1;
            }
            else
            {
#if DEBUG_SQLITE
              printf("select goods OK!\n");
#endif
              sqlite3_close(db);
            }
       env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type = (int)callback_val[1];
		env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count = (int)callback_val[2];

		all_info_RT.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type = (int)callback_val[1];
		all_info_RT.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count = (int)callback_val[2];
#if DEBUG_SQLITE 
		printf ("goods_s.goods_type = %d\n", env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_type);
		printf ("goods_s.goods_count = %d\n", env_info_clien_addr_s.storage_no[storageNum_t].goods_info[goodsKinds_t].goods_count);
		printf ("---------------------------------\n");
#endif
	}

	return 0;
      
}

int getAllGoods (void)
{
#if DEBUG_SQLITE
       printf("getAllGoods ok\n");
#endif
    return 0;
}

/*
**数据库模块任务
*/
void sqlite_task (struct env_info_clien_addr env_info_clien_addr_t, \
                  struct sqlite_operation sqlite_operation_t, int storageNum_t, int goodsKinds_t)
{
#if DEBUG_SQLITE
	printf ("sqlite_task!!!\n");  //测试部分，可删。
#endif

//int storage_num;
  if(0 == sqlite_operation_t.table_select_mask) //0表示对environment表进行操作
  {
      if (0 == sqlite_operation_t.env_operation_mask)  //0表示重新设置环境参数
      {
		updateEnv (env_info_clien_addr_t, storageNum_t);
	  }
	  else if (1 == sqlite_operation_t.env_operation_mask)  //1表示获得环境参数
      {
          getEnv(&env_info_clien_addr_t, storageNum_t);
      }    
  }
  else if(1 == sqlite_operation_t.table_select_mask) //1表示对colect_env表进行操作
  {
     if(0 == sqlite_operation_t.table_operation_mask)  //0表示对表colect_env进行插入数据的操作
	 {
            insertCollect_env (env_info_clien_addr_t, storageNum_t);
     }
     else if(1 == sqlite_operation_t.table_operation_mask) //1表示获得colect_env表数据的操作(非实时)
     {
         getCollect_env (env_info_clien_addr_t.storage_no[storageNum_t].samplingTime, storageNum_t);
     }
     else if (2 == sqlite_operation_t.table_operation_mask)  //1表示获得colect_env表当前数据的操作(实时)
	{
        getCollect_Current_env (storageNum_t);
    }
  }
  else if (2 == sqlite_operation_t.table_select_mask)  //2表示对goods表进行操作
  {
      if (0 == sqlite_operation_t.goods_operation_mask)  //0表示有新的货物入库
		{
            if (0 == viewGoods(storageNum_t, goodsKinds_t)) //入库货物种类仓库中没有
			{
				insertGoods (env_info_clien_addr_t, storageNum_t, goodsKinds_t);
			}
            else
			{
				addGoods (env_info_clien_addr_t, storageNum_t, goodsKinds_t);
			}
        }
       else if (1 == sqlite_operation_t.goods_operation_mask)  //1表示有货物出库
        {
           reduceGoods (env_info_clien_addr_t, storageNum_t, goodsKinds_t);
        }
        else if (2 == sqlite_operation_t.goods_operation_mask)  //2表示获得指定goodsId的货物信息
		{
			getGoods (storageNum_t, goodsKinds_t);
		}
        else if (3 == sqlite_operation_t.goods_operation_mask)  //3表示获得所有货物信息
		{
			getAllGoods ();
		}
  }

}

void *pthread_sqlite (void *arg)
{
    slinkHead = sqlite_CreateEmptyLinklist();
    slinklist buf = NULL;
#if DEBUG_SQLITE
    printf("slinkHead ok\n");
#endif
    printf("pthread_sqlite is ok\n");
    while(1)
    {
        pthread_mutex_lock(&mutex_sqlite);
        pthread_cond_wait(&cond_sqlite, &mutex_sqlite);
        pthread_mutex_unlock(&mutex_sqlite);
#if DEBUG_SQLITE
        printf("pthread_sqlite is up\n");
#endif
      while(1)
      {
           pthread_mutex_lock(&mutex_slinklist);
           if((buf = sqlite_GetLinknode(slinkHead)) == NULL)
           {
               pthread_mutex_unlock(&mutex_slinklist);
               break;
           }
           pthread_mutex_unlock(&mutex_slinklist);
           sqlite_task(buf->data_link, buf->data, buf->storageNum, buf->goodsKinds);
           free(buf);
           buf = NULL;
      }
#if DEBUG_SQLITE
       printf("info come on\n");
#endif
    }
}