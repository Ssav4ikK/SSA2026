#include "LIBS.h"

namespace ERROR {
	Error errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
        ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
        ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
        ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
        ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

        ERROR_ENTRY(100, "Параметр -in должен быть задан"),
        ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
        ERROR_ENTRY(104, "Превышена длина входного параметра"),
        ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
        ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),

        ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
        ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
        ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
        ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
        ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),

        ERROR_ENTRY(120, "Ошибка при создании таблицы лексем: Заданная ёмксоть больше максимально возможной"),
        ERROR_ENTRY(121, "Ошибка при добавлении сущности: Превышена ёмкость таблицы"),
        ERROR_ENTRY(122, "Ошибка при получении сущности: Не верный номер строки"),
        ERROR_ENTRY_NODEF(123), ERROR_ENTRY_NODEF(124), ERROR_ENTRY_NODEF(125),
        ERROR_ENTRY_NODEF(126), ERROR_ENTRY_NODEF(127), ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),

        ERROR_ENTRY(130, "Ошибка при создании таблицы идентификаторов: Заданная ёмксоть больше максимально возможной"),
        ERROR_ENTRY(131, "Ошибка при добавлении сущности в таблицу идентификатор: Превышена ёмкость таблицы"),
        ERROR_ENTRY(132, "Ошибка при получении сущности из таблицы идентификаторов: Не верный номер строки"),
        ERROR_ENTRY_NODEF(133), ERROR_ENTRY_NODEF(134), ERROR_ENTRY_NODEF(135),
        ERROR_ENTRY_NODEF(136), ERROR_ENTRY_NODEF(137), ERROR_ENTRY_NODEF(138), ERROR_ENTRY_NODEF(139),
        
        ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
        ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
        ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
        ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
    

    Error getError(int id) {
        if (id < 0 || id >= ERROR_MAX_ENTRY) {
            return errors[0];
        }
        return errors[id];
    }

    Error getErrorPlace(int id, int line, int col) {
        if (id < 0 || id >= ERROR_MAX_ENTRY) {
            return errors[0];
        }
        Error er = errors[id];
        er.place.line = line;
        er.place.col = col;
        return er;
    }
}