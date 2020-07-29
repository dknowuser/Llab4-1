/*
 * main.cpp
 *
 *  Created on: 9 сент. 2018 г.
 *      Author: wetfloor
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/resource.h>

using namespace std;

int pid = 0;
sched_param* params = 0;

void* threadFunction(void* textFile)
{
	int sched_class = sched_getscheduler(pid);

	cout << endl;
	cout << "Класс планирования процесса: " << sched_class;
	switch(sched_class) {
	case SCHED_FIFO:
		cout << " (SCHED_FIFO)" << endl;
		break;
	case SCHED_RR:
		cout << " (SCHED_RR)" << endl;
		break;
	case SCHED_OTHER:
		cout << " (SCHED_OTHER)" << endl;
		break;
	};

	cout << "Текущий приоритет процесса: " << getpriority(PRIO_PROCESS, pid) << endl;
	cout << "Текущий минимальный приоритет процесса: " << sched_get_priority_min(sched_class) << endl;
	cout << "Текущий максимальный приоритет процесса: " << sched_get_priority_max(sched_class) << endl;
	cout << "Содержимое файла:" << endl;

	int readChar = fgetc((FILE*)textFile);

	while(readChar != EOF) {
		cout << (char)readChar;
		readChar = fgetc((FILE*)textFile);
	};
	cout << endl;

	fclose((FILE*)textFile);
	return 0;
};

int main(void)
{
	//Открываем текстовый файл
	FILE* textFile = fopen("/home/wetfloor/Документы/etc/textFile", "r+");
	pid = getpid();
	params = new sched_param;

	if(textFile) {
		cout << "Текстовый файл успешно открыт." << endl;

		//Создаём поток
		pthread_t thread = 0;
		if(!pthread_create(&thread, NULL, threadFunction, textFile))
		{
			cout << "Поток успешно создан." << endl;

			//Ждём, пока поток завершит свою работу
			if(!pthread_join(thread, 0)) {
				cout << "Поток завершил свою работу." << endl;

				cout << "Текущий приоритет процесса: " << getpriority(PRIO_PROCESS, pid) << endl;

				fputc('x', textFile);
				if(ferror(textFile)) {
					cout << "Текстовый файл был закрыт." << endl;
				}
				else {
					cout << "Текстовый файл не был закрыт." << endl;
					fclose(textFile);
				};
			}
			else {
				cout << "Ошибка при ожидании завершения потока." << endl;
			};
		}
		else
		{
			cout << "Ошибка при создании потока." << endl;
		};
	};

	while(true);
	return 0;
};


