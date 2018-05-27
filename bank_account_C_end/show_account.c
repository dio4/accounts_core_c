//прототип функции показа аккаунтов для основной программы
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

