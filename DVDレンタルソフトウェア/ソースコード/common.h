/*
 * ファイル名	: common.h
 * 機能			: 共通の定義を行う
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  
 */

#ifndef COMMON_H
#define COMMON_H

/* #defineマクロ */
/* 真偽 */
#define TRUE							1		/* 真 */
#define FALSE							0		/* 偽 */

/* レコード終端 */
#define REC_DELIMITER_SIZE				10		/* レコード区切り文字長さ */

/* 文字列終端 */
#define STRING_TERMINATOR_SIZE			1		/* 文字列終端文字長さ */

/* バッファサイズ */
/* GetYesNo関数 */
#define GETYESNO_BUF_SIZE				4		/* GetYesNo関数 文字列入力バッファサイズ */

/* GetStr関数 */
#define GETSTR_BUF_SIZE					101		/* GetStr関数 文字列入力バッファサイズ */

/* GetNumStr */
#define GETNUMSTR_BUF_SIZE				17		/* GetNumStr 文字列入力バッファサイズ */
	
/* GetIntStr */
#define GETINTSTR_BUF_SIZE				17		/* GetIntStr 文字列入力バッファサイズ */
	
/* InputYMD */
#define INPUTYMD_YEAR_BUF_SIZE			8		/* InputYMD 年の文字列入力バッファサイズ */
#define INPUTYMD_MONTH_BUF_SIZE			8		/* InputYMD 月の文字列入力バッファサイズ */
#define INPUTYMD_DAY_BUF_SIZE			8		/* InputYMD 日の文字列入力バッファサイズ */
#define INPUTYMD_BUF_SIZE				16		/* InputYMD  8桁の日付文字列格納用バッファサイズ */

/* GetDateStr */
#define GETDATESTR_BUF_SIZE				17		/* GetDateStr 文字列入力バッファ */
	
/* IsDateStr */
#define ISDATESTR_YEAR_BUF_SIZE			8		/* IsDateStr 年の文字列格納用バッファサイズ */
#define ISDATESTR_MONTH_BUF_SIZE		8		/* IsDateStr 月の文字列格納用バッファサイズ */
#define ISDATESTR_DAY_BUF_SIZE			8		/* IsDateStr 日の文字列格納用バッファサイズ */

/* ConvFormatDate */
#define CONVFORMATDATE_YEAR_BUF_SIZE	8		/* ConvFormatDate 年の文字列格納用バッファサイズ */
#define CONVFORMATDATE_MONTH_BUF_SIZE	8		/* ConvFormatDate 月の文字列格納用バッファサイズ */
#define CONVFORMATDATE_DAY_BUF_SIZE		8		/* ConvFormatDate 日の文字列格納用バッファサイズ */
	
/* DiffDate */
#define DIFFDATE_YEAR_BUF_SIZE		(4 + 1)		/* DiffDate 年の文字列格納用バッファサイズ */
#define DIFFDATE_MONTH_BUF_SIZE		(2 + 1)		/* DiffDate 月の文字列格納用バッファサイズ */
#define DIFFDATE_DAY_BUF_SIZE		(2 + 1)		/* DiffDate 日の文字列格納用バッファサイズ */

/* GetMenuNum */
#define GETMENUNUM_BUF_SIZE		1				/* GetMenuNum メニュー番号入力バッファ */

/* 日付操作 */
#define YEAR_SIZE				4				/* 日付文字列 年 */
#define MONTH_SIZE				2				/* 日付文字列 月 */
#define DAY_SIZE				2				/* 日付文字列 日 */
#define DATE_STR_SIZE			8				/* 日付文字列 年月日 */
#define BASE_YEAR				1900			/* 日付関数用基準年 */
#define BASE_MONTH				1				/* 日付関数用基準月 */
#define MIN_YEAR				1800			/* 日付関数取扱年 最小値*/
#define MAX_YEAR				2100			/* 日付関数用扱年 最大値 */
#define MIN_MONTH				1				/* 日付関数取扱年 最小値*/
#define MAX_MONTH				12				/* 日付関数用扱年 最大値 */
#define MIN_DAY					1				/* 日付関数取扱年 最小値*/
#define MAX_DAY					31				/* 日付関数用扱年 最大値 */
#define SECOND_OF_DAY			(3600 * 24)		/* 日付関数用 1日当たりの秒数 */

/* メニュー */
#define ENTER_NUM				99				/* メニューへ入る番号 */
#define REGIST_NUM				1				/* 連続貸出/返却処理の終了番号（レンタル） */
#define EXIT_NUM				0				/* メニューを抜ける番号 */
#define NULL_NUM				10				/* NULL番号（空文字） */
#define MENU_SEL_NUM			1				/* メニュー選択番号の桁数 */
#define MENU_NUM				10				/* メニュー選択肢の最大数 */

/* 商品情報ファイル：貸出状況 */
#define GOODS_LEND_NO_VAL		"0"				/* 貸出可の内部値 */
#define GOODS_LEND_YES_VAL		"1"				/* 貸出中の内部値 */
#define GOODS_LEND_NO_NAME		"貸出可"		/* 貸出可の表示文字 */
#define GOODS_LEND_YES_NAME		"貸出中"		/* 貸出中の表示文字 */
#define GOODS_RET_NO_NAME		"返却済"		/* 返却済の表示文字 */
#define GOODS_RET_YES_NAME		"返却可"		/* 返却中の表示文字 */

/* 商品情報ファイル：保管場所 */
#define GOODS_KEEP_WARE_VAL		"0"				/* 倉庫の内部値 */
#define GOODS_KEEP_SHOP_VAL		"1"				/* 店頭の内部値 */
#define GOODS_KEEP_WARE_NAME	"倉庫"			/* 倉庫の表示文字 */
#define GOODS_KEEP_SHOP_NAME	"店頭"			/* 店頭の表示文字 */

/* structタグ定義 */
/* タイトル */
typedef struct {
	char* title;					/* タイトル文字列へのポインタ */
}TITLE;

/* メニュー選択肢 */
typedef struct {
	int num;						/* メニュー選択番号 */
	char* str;						/* メニュー選択文字列へのポインタ */
}MENU_SEL;

/* メニュー表示 */
typedef struct {
	TITLE title;					/* タイトル文字列へのポインタ */
	MENU_SEL sel[MENU_NUM];			/* メニュー選択肢 */
}MENU;

/* 関数プロトタイプ宣言 */
/* 文字関係 */
extern int GetYesNo(const char *s);
extern void GetAnyKey(const char *s);
extern int GetStr(char *s, const int size);
extern int GetNumStr(char *s, const int size);
extern int GetIntStr(char *s, const int s1, const int s2);
extern void TrimSpace(char s[]);

/* 整数関係 */
extern int GetFigure(int num);

/* 日付関係 */
extern void InputYMD(char *date);
extern int GetDateStr(char *date);
extern void GetToday(const int y, const int m, const int d, char *date);
extern int IsDateStr(const char *date);
extern void ConvFormatDate(char *dest, const char *src);
extern int DiffDate(const char *fromdate,const char *todate);

/* メニュー関係 */
extern int GetMenuNum(const MENU *m);
extern void DisplayMenu(const MENU *m);
extern void DisplayTitle(const TITLE* t);

/* レンタル処理関係 */
extern char *ConvLendStr(const char *s);
extern char *ConvRetStr(const char *s);
extern char *ConvKeepStr(const char *s);

#endif		/* end COMMON_H */

