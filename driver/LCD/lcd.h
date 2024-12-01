#ifndef __LCD_H
#define __LCD_H
#include "stm32f4xx.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////
// STM32F4����ģ��-�⺯���汾
// �Ա����̣�http://mcudev.taobao.com
//********************************************************************************
// V1.2�޸�˵��
// ֧����SPFD5408������,�����Һ��IDֱ�Ӵ�ӡ��HEX��ʽ.����鿴LCD����IC.
// V1.3
// �����˿���IO��֧��
// �޸��˱�����Ƶļ��ԣ�������V1.8���Ժ�Ŀ�����汾��
// ����1.8�汾֮ǰ(������1.8)��Һ��ģ��,���޸�LCD_Init������LCD_LED=1;ΪLCD_LED=1;
// V1.4
// �޸���LCD_ShowChar������ʹ�û��㹦�ܻ��ַ���
// �����˺�������ʾ��֧��
// V1.5 20110730
// 1,�޸���B505Һ������ɫ�����bug.
// 2,�޸��˿���IO�������������÷�ʽ.
// V1.6 20111116
// 1,�����LGDP4535Һ��������֧��
// V1.7 20120713
// 1,����LCD_RD_DATA����
// 2,���Ӷ�ILI9341��֧��
// 3,����ILI9325�Ķ�����������
// 4,����LCD_Scan_Dir����(����ʹ��)
// 6,�����޸��˲���ԭ���ĺ���,����Ӧ9341�Ĳ���
// V1.8 20120905
// 1,����LCD��Ҫ�������ýṹ��lcddev
// 2,����LCD_Display_Dir����,֧�����ߺ������л�
// V1.9 20120911
// 1,����RM68042������ID:6804��������6804��֧�ֺ�����ʾ����ԭ�򣺸ı�ɨ�跽ʽ��
// ����6804��������ʧЧ���Թ��ܶ෽�������У���ʱ�޽⡣
// V2.0 20120924
// �ڲ�Ӳ����λ�������,ILI9341��ID��ȡ�ᱻ�����9300,�޸�LCD_Init,���޷�ʶ��
// �����������IDΪ9300/�Ƿ�ID��,ǿ��ָ������ICΪILI9341��ִ��9341�ĳ�ʼ����
// V2.1 20120930
// ����ILI9325����ɫ��bug��
// V2.2 20121007
// ����LCD_Scan_Dir��bug��
// V2.3 20130120
// ����6804֧�ֺ�����ʾ
// V2.4 20131120
// 1,����NT35310��ID:5310����������֧��
// 2,����LCD_Set_Window����,�������ô���,�Կ������,�Ƚ�����,���Ǹú����ں���ʱ,��֧��6804.
// V2.5 20140211
// 1,����NT35510��ID:5510����������֧��
// V2.6 20140504
// 1,����ASCII 24*24�����֧��(���������û�������������)
// 2,�޸Ĳ��ֺ�������,��֧��MDK -O2�Ż�
// 3,���9341/35310/35510,дʱ������Ϊ���,�����ܵ�����ٶ�
// 4,ȥ����SSD1289��֧��,��Ϊ1289ʵ����̫����,������Ҫ1us...��ֱ����.���ʺ�F4ʹ��
// 5,����68042��C505��IC�Ķ���ɫ������bug.
// V2.7 20140514
// 1,����LCD_Color_Fill������һ��bug.
//////////////////////////////////////////////////////////////////////////////////

// LCD��Ҫ������
typedef struct
{
    uint16_t width;   // LCD ����
    uint16_t height;  // LCD �߶�
    uint16_t id;      // LCD ID
    uint8_t dir;      // ���������������ƣ�0��������1��������
    uint16_t wramcmd; // ��ʼдgramָ��
    uint16_t setxcmd; // ����x����ָ��
    uint16_t setycmd; // ����y����ָ��
} _lcd_dev;

// LCD����
extern _lcd_dev lcddev; // ����LCD��Ҫ����
// LCD�Ļ�����ɫ�ͱ���ɫ
extern uint16_t POINT_COLOR; // Ĭ�Ϻ�ɫ
extern uint16_t BACK_COLOR;  // ������ɫ.Ĭ��Ϊ��ɫ

//////////////////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���----------------
#define LCD_LED(__flag)                                    \
    {                                                      \
        GPIO_InitTypeDef GPIO_InitStructure;               \
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      \
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; \
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     \
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;          \
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   \
        GPIO_Init(GPIOB, &GPIO_InitStructure);             \
        if (__flag == 1)                                   \
        {                                                  \
            GPIO_SetBits(GPIOB, GPIO_Pin_1);               \
        }                                                  \
        if (__flag == 0)                                   \
        {                                                  \
            GPIO_ResetBits(GPIOB, GPIO_Pin_1);             \
        }                                                  \
    } // LCD����    		 PB1
// LCD��ַ�ṹ��
typedef struct
{
    uint16_t LCD_REG;
    uint16_t LCD_RAM;
} LCD_TypeDef;
// ʹ��NOR/SRAM�� Bank1.sector1,��ַλHADDR[27,26]=00   A18��Ϊ��������������
// ע������ʱSTM32�ڲ�������һλ����!
#define LCD_BASE ((uint32_t)(0x60000000 | 0x00007FFFE))
#define LCD ((LCD_TypeDef *)LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////

// ɨ�跽����
#define L2R_U2D 0 // ������,���ϵ���
#define L2R_D2U 1 // ������,���µ���
#define R2L_U2D 2 // ���ҵ���,���ϵ���
#define R2L_D2U 3 // ���ҵ���,���µ���

#define U2D_L2R 4 // ���ϵ���,������
#define U2D_R2L 5 // ���ϵ���,���ҵ���
#define D2U_L2R 6 // ���µ���,������
#define D2U_R2L 7 // ���µ���,���ҵ���

#define DFT_SCAN_DIR L2R_U2D // Ĭ�ϵ�ɨ�跽��

// ������ɫ
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // ��ɫ
#define BRRED 0XFC07 // �غ�ɫ
#define GRAY 0X8430  // ��ɫ
// GUI��ɫ

#define DARKBLUE 0X01CF  // ����ɫ
#define LIGHTBLUE 0X7D7C // ǳ��ɫ
#define GRAYBLUE 0X5458  // ����ɫ
// ������ɫΪPANEL����ɫ

#define LIGHTGREEN 0X841F // ǳ��ɫ
// #define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 0XC618 // ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE 0XA651 // ǳ����ɫ(�м����ɫ)
#define LBBLUE 0X2B12    // ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

void LCD_Init(void);                                                                                    // ��ʼ��
void LCD_DisplayOn(void);                                                                               // ����ʾ
void LCD_DisplayOff(void);                                                                              // ����ʾ
void LCD_Clear(uint16_t Color);                                                                         // ����
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);                                                       // ���ù��
void LCD_DrawPoint(uint16_t x, uint16_t y);                                                             // ����
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color);                                        // ���ٻ���
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y);                                                         // ����
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r);                                              // ��Բ
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                                  // ����
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                             // ������
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);                      // ��䵥ɫ
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);               // ���ָ����ɫ
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);                     // ��ʾһ���ַ�
void LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size);                      // ��ʾһ������
void LCD_ShowxNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);       // ��ʾ ����
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p); // ��ʾһ���ַ���,12/16����

void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_Scan_Dir(uint8_t dir);                                                 // ������ɨ�跽��
void LCD_Display_Dir(uint8_t dir);                                              // ������Ļ��ʾ����
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height); // ���ô���
// 9320/9325 LCD�Ĵ���
#define R0 0x00
#define R1 0x01
#define R2 0x02
#define R3 0x03
#define R4 0x04
#define R5 0x05
#define R6 0x06
#define R7 0x07
#define R8 0x08
#define R9 0x09
#define R10 0x0A
#define R12 0x0C
#define R13 0x0D
#define R14 0x0E
#define R15 0x0F
#define R16 0x10
#define R17 0x11
#define R18 0x12
#define R19 0x13
#define R20 0x14
#define R21 0x15
#define R22 0x16
#define R23 0x17
#define R24 0x18
#define R25 0x19
#define R26 0x1A
#define R27 0x1B
#define R28 0x1C
#define R29 0x1D
#define R30 0x1E
#define R31 0x1F
#define R32 0x20
#define R33 0x21
#define R34 0x22
#define R36 0x24
#define R37 0x25
#define R40 0x28
#define R41 0x29
#define R43 0x2B
#define R45 0x2D
#define R48 0x30
#define R49 0x31
#define R50 0x32
#define R51 0x33
#define R52 0x34
#define R53 0x35
#define R54 0x36
#define R55 0x37
#define R56 0x38
#define R57 0x39
#define R59 0x3B
#define R60 0x3C
#define R61 0x3D
#define R62 0x3E
#define R63 0x3F
#define R64 0x40
#define R65 0x41
#define R66 0x42
#define R67 0x43
#define R68 0x44
#define R69 0x45
#define R70 0x46
#define R71 0x47
#define R72 0x48
#define R73 0x49
#define R74 0x4A
#define R75 0x4B
#define R76 0x4C
#define R77 0x4D
#define R78 0x4E
#define R79 0x4F
#define R80 0x50
#define R81 0x51
#define R82 0x52
#define R83 0x53
#define R96 0x60
#define R97 0x61
#define R106 0x6A
#define R118 0x76
#define R128 0x80
#define R129 0x81
#define R130 0x82
#define R131 0x83
#define R132 0x84
#define R133 0x85
#define R134 0x86
#define R135 0x87
#define R136 0x88
#define R137 0x89
#define R139 0x8B
#define R140 0x8C
#define R141 0x8D
#define R143 0x8F
#define R144 0x90
#define R145 0x91
#define R146 0x92
#define R147 0x93
#define R148 0x94
#define R149 0x95
#define R150 0x96
#define R151 0x97
#define R152 0x98
#define R153 0x99
#define R154 0x9A
#define R157 0x9D
#define R192 0xC0
#define R193 0xC1
#define R229 0xE5
#endif
