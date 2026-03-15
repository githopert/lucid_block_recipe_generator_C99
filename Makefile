build:
	tcc -Wall -Wfatal-errors -std=c99 main.c table.c fuser.c -o app.exe