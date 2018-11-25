/************************************************************************
*                                                                       *
* FileName:                                                             *
*            crc.h                                                      *
*                                                                       *
* Projecto:                                                             *
*            PR710_SDH_RA                                               *
*                                                                       *
* Descri��o: Header file para o modulo de software crc.c                *
*                                                                       *
*                                                                       *
* Hist�rico:                                                            *
*            22 Janeiro 2003 - primeira vers�o                          *
*                                                                       *
*                                                                       *
* Autor: Celso Lemos / Nuno Pires                                       *
************************************************************************/
#ifndef CRC_H
#define CRC_H

#include <stdint.h>

/*********************************************************************************
* Fun��o:         getCRC16                                                       *
*                                                                                *
* Descri��o:      Retorna o crc de um determinado bloco de bytes coniguos        *
*                                                                                *
* Parametros:                                                                    *
*			  startaddr: endere�o do primeiro byte do bloco                      *
*             size:      numero de bytes do bloco                                *
*                                                                                *
* retorno:                                                                       *
*                Retorna uma word com o crc calculado                            *
*                                                                                *
* historico:                                                                     *
*         9 Julho de 2002                                                     *
*                                                                                *
* Autor: Nuno Pires                                                              *
*                                                                                *
*********************************************************************************/
#ifdef __cplusplus
extern "C"
#else
extern
#endif
uint16_t getCRC16(uint8_t *startaddr,uint32_t size);

#ifdef __cplusplus
extern "C"
#else
extern
#endif
uint16_t getCRC16_from_file(FILE *fp, uint32_t size);

#ifdef __cplusplus
extern "C"
#else
extern
#endif
uint16_t getCRC16_next(uint8_t *startaddr,uint32_t size,uint16_t startcrc);

#endif /* CRC_H */
