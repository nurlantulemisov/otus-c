FROM ubuntu : latest

#Обновление пакетов и установка необходимых инструментов
		RUN apt
	      - get update &&apt
	      - get install
	      - y build
	      - essential cmake git nasm

#Установка любых других необходимых инструментов или библиотек

#Убедитесь, что рабочая директория существует
		    WORKDIR
		  / usr / src
		  / app

#Копирование исходных файлов(asm кода) в контейнер
		      COPY..

#RUN mv / usr / src / app /.config / nvim / / root /.config / nvim /
#Команда по умолчанию, которая может быть переопределена при запуске контейнера
		    CMD["bash"]
