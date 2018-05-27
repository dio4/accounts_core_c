/*программа имитирует систему банковских счетов. Добавляет аккаунт, изменяет, 
 * работает с вкладами! Использует файлы произв. доступа. 
 * ВЕРИФИКАЦИЯ ввода + функц. вывода всех аккаунтов  
 * Файл accounts_store (исполнимый) дб запущен ДО основной программы - создает файл данных
 *  бинарный "credit.dat" и заполняет его пустыми перем. типа структура 
 *  в кол. 100 штук. Основной зайпускающий файл - accounts_run
 */

//добавить избирательную сортировку(напр. по размеру вклада, по имени, по N аккаунта) и поиск.

#include <stdio.h>
/*в файле "credit.dat" (см. Fig11_11.c) создается 100 пустых переменных blankClient типа struct clientData */
struct clientData { 
   int acctNum;
   char lastName[ 15 ];
   char firstName[ 10 ];
   double balance;
};

//прототипы 
int enterChoice( void );
int verification(void);
void textFile( FILE * );
void updateRecord( FILE * );
void newRecord( FILE * );
void deleteRecord( FILE * );
void show_account(FILE *); //стр 206


int main()
{ 
   FILE *cfPtr;
   int choice;

   if ( ( cfPtr = fopen( "credit.dat", "r+" ) ) == NULL )
      printf( "не могу открыть файл.\n" );
   else { 

      while ( ( choice = enterChoice() ) != 6 ) { //enterChoice() стр 166

         switch ( choice ) { 
            case 1:
               textFile( cfPtr );//стр55
               break;
            case 2:
               updateRecord( cfPtr );
               break;
            case 3:
               newRecord( cfPtr );
               break;
            case 4:
               deleteRecord( cfPtr );
               break;
            case 5:
            	show_account(cfPtr);
            	break;
         }
      }

      fclose( cfPtr );
   }
   printf("Программа завершена\n");
   return 0;
}

void textFile( FILE *readPtr ) //записывает данные из "credit.dat" в "accounts.txt" если номер аккаунта != 0 (не пустой аккаунт)
{ 
   FILE *writePtr;
   struct clientData client = { 0, "", "", 0.0 };
// readPtr указатель на открытый файл credit.dat а writePtr - указатель на открытый файл accounts.txt
   if ( ( writePtr = fopen( "accounts.txt", "w" ) ) == NULL )
      printf( "File could not be opened.\n" );
   else { 
      rewind( readPtr );
      fprintf( writePtr, "%-6s%-16s%-11s%10s\n", 
              "Acct", "Last Name", "First Name","Balance" );

      while ( !feof( readPtr ) ) { 
         fread( &client, sizeof( struct clientData ), 1, 
                readPtr );

         if ( client.acctNum != 0 )
            fprintf( writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum, client.lastName,
                    client.firstName, client.balance );
      }

	  fclose( writePtr );
   }

}

void updateRecord( FILE *fPtr )
{ 
   int account;
   double transaction;
   char ch;
   struct clientData client = { 0, "", "", 0.0 };
   printf( "Для изменения аккаунта введите его номер от ( 1 - 100 ): " );
   account = verification();
   /* верификация ввода 
   	while (scanf("%d", &account) != 1 || account < 1 || account > 100)
	{
		while ((ch = getchar()) != '\n')
			putchar(ch);
		printf(" не является допустимым номером. Повторите ввод: \n");
	}
	*/
   fseek( fPtr, 
          ( account - 1 ) * sizeof( struct clientData ), 
          SEEK_SET );
   fread( &client, sizeof( struct clientData ), 1, fPtr );

   if ( client.acctNum == 0 )
      printf( "Аккаунт #%d не имеет информации.\n", account );
   else { 
      printf( "%-6d%-16s%-11s%10.2f\n\n", //выводим имеющиеся данные для введенного номера аккаунта
             client.acctNum, client.lastName, 
             client.firstName, client.balance );
      printf( "введите прибавку ( + ) или сумму выплат к счету ( - ): " );
      scanf( "%lf", &transaction );
      client.balance += transaction;//меняем баланс на величину транзакции
      printf( "%-6d%-16s%-11s%10.2f\n", //снова выводим уже измененные данные
             client.acctNum, client.lastName, 
             client.firstName, client.balance );
      fseek( fPtr, 
             ( account - 1 ) * sizeof( struct clientData ), 
             SEEK_SET ); //возвращаем указатель в файле на исходную позицию
      fwrite( &client, sizeof( struct clientData ), 1, 
              fPtr );//записываем в файл внесенные изменения
   }
}

void deleteRecord( FILE *fPtr )//принимает указатель на "credit.dat"
{ 
   struct clientData client, 
                     blankClient = { 0, "", "", 0 };
   int accountNum;

   printf( "Введите номер аккаунта для "
           "удаления в приделах ( 1 - 100 ): " );
   //scanf( "%d", &accountNum );
   accountNum = verification();
   fseek( fPtr, 
          ( accountNum - 1 ) * sizeof( struct clientData ), 
          SEEK_SET );
   fread( &client, sizeof( struct clientData ), 1, fPtr );

   if ( client.acctNum == 0 )
      printf( "Аккаунт %d не существует.\n", accountNum );
   else { 
      fseek( fPtr, 
         ( accountNum - 1 ) * sizeof( struct clientData ), 
         SEEK_SET );
      fwrite( &blankClient, 
              sizeof( struct clientData ), 1, fPtr );//записываем пустой аккаунт на место, соответств. введенному номеру
   }
}

void newRecord( FILE *fPtr )//принимает указатель на "credit.dat"
{ 
   struct clientData client = { 0, "", "", 0.0 };//созд. переменнная типа struct clientData с пустыми параментрами
   int accountNum;
   printf( "Введите новый номер аккаунта ( 1 - 100 ): " );
   //scanf( "%d", &accountNum );
   accountNum = verification();
   
   fseek( fPtr, 
          ( accountNum - 1 ) * sizeof( struct clientData ), 
          SEEK_SET );
   fread( &client, sizeof( struct clientData ), 1, fPtr );

   if ( client.acctNum != 0 )
      printf( "аккаунт #%d уже содержит информацию!\n",
             client.acctNum );
   else { 
      printf( "Введите отчество, имя, баланс\n? " );
      scanf( "%s%s%lf", &client.lastName, &client.firstName, 
            &client.balance );
      client.acctNum = accountNum;
      fseek( fPtr, ( client.acctNum - 1 ) * 
            sizeof( struct clientData ), SEEK_SET );
      fwrite( &client, 
              sizeof( struct clientData ), 1, fPtr );
   }
}

int enterChoice( void )
{ 
   int menuChoice;

   printf( "\nВыберете пункт меню:\n"
      "1 - сохранить внесенные изменения в файл \n"
      "    \"accounts.txt\" для вывода\n"
      "2 - изменить аккаунт\n"
      "3 - добавить новый аккаунт\n"
      "4 - удалить аккаунт\n"
      "5 - вывести имеющиеся аккаунты\n"
      "6 - завершить программу\n? " );
   //scanf( "%d", &menuChoice );
   menuChoice = verification();
   return menuChoice;
}


void show_account( FILE *readPtr )
{ 
   //FILE *writePtr;
   struct clientData client = { 0, "", "", 0.0 };

   /*if ( ( writePtr = fopen( "accounts.txt", "w" ) ) == NULL )
      printf( "File could not be opened.\n" );
   else { */
      rewind( readPtr );
      printf( "%-6s%-16s%-11s%10s\n", 
              "Acct", "Last Name", "First Name","Balance\n" );

      while ( !feof( readPtr ) ) { 
         fread( &client, sizeof( struct clientData ), 1, 
                readPtr );

         if ( client.acctNum != 0 )
            printf("%-6d%-16s%-11s%10.2f\n",
                    client.acctNum, client.lastName,
                    client.firstName, client.balance );
      }
}

int verification(void)
{
	/* верификация ввода */
	char ch;
	int my_input;
   	while (scanf("%d", &my_input) != 1 || my_input < 1 || my_input > 100)
	{
		while ((ch = getchar()) != '\n')
			putchar(ch);
		printf(" не является допустимым номером. Повторите ввод: \n");
	}
	return my_input;
}

