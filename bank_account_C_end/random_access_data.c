/* 
   Создаем файл произвольного доступа */
/*программа имитирует банковскую систему с записью информации в файл с произв. доступом
имеет 6 функций:
							1 - сохранить внесенные изменения в файл 
								"accounts.txt" для вывода
							2 - изменить аккаунт
							3 - добавить новый аккаунт
							4 - удалить аккаунт
							5 - вывести имеющиеся аккаунты
							6 - завершить программу
							и простую верификацию ввода данных.
							
Первым должен быть запущен ЭТОТ файл для созд. хранилища credit.dat.
Рабочий файл - ./accounts_core (исполнимый)
*/

#include <stdio.h>

struct clientData { 
   int acctNum;
   char lastName[ 15 ];
   char firstName[ 10 ];
   double balance;
};

int main()
{ 
   int i;
   struct clientData blankClient = { 0, "", "", 0.0 };
   FILE *cfPtr;
/*
Для таких операционных систем, как Unix и Linux, поддерживающих только один
тип файла , режимы , обозначенные буквой Ь эквивалентны соответствующим режимам,
 в обозначении которых буква Ь отсутствует.
*/
   if ( ( cfPtr = fopen( "credit.dat", "w" ) ) == NULL )//файл по умолч. бинарный в UNIX
      printf( "File could not be opened.\n" );
   else { 

      for ( i = 1; i <= 100; i++ )
         fwrite( &blankClient, 
                sizeof( struct clientData ), 1, cfPtr );

      fclose ( cfPtr );
   }

   return 0;
}

