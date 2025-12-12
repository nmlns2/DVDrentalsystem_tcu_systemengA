/*
 * ファイル名	: common.c
 * 機能			: 共通の定義を行う
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  2012/02/14 V02L1
 */

/* ヘッダーの取り込み */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "common.h"

/* static関数宣言 */
static int IsNumStr(const char *s, const int size);

/*
 * 関数名			: int GetYesNo(const char *s)
 * 機能				: YまたはNを選択する
 * 入力パラメータ	: const char *s		:表示文字
 * 出力パラメータ	: なし
 * 戻り値			: int	:Yを入力したときTRUE / Nを入力したときFALSE
 * 特記事項			: Y/N（y/n）以外を入力した場合には再入力を促す
 */
int GetYesNo(const char *s)
{
	int retVal = FALSE;						/* 戻り値 */
	char buf[GETYESNO_BUF_SIZE], in;
	int len;								/* 入力した文字列の長さ */
	
	while(TRUE) {
		/* タイトルを表示 */
        printf("\n%s（Y/N）＞　", s);
		
        /* キーボードからの入力を文字列として取得する */
		fflush(stdin);						/* 入力バッファをクリア */
		fgets(buf, sizeof(buf), stdin);		/* 文字を入力 */
		len = strlen(buf);
		buf[--len] = '\0';					/* 改行コードを消す */
		fflush(stdin);						/* 入力を超えた分の文字をクリア */
	
		/* 文字列をチェックする */
		if(len >= 2) {						/* 2桁以上入力 */
			/* DO NOTHING */
		} else {
			in = tolower(buf[0]);			/* 英小文字に変換 */
			/* 文字をチェックする */
			if(in == 'y') {
				retVal = TRUE;
				break;
			} else if(in == 'n') {
				retVal = FALSE;
				break;
			} else {
				/* DO NOTHING */
			}
		}
	}
	return retVal;
}

/*
 * 関数名			: int GetStr(char *s, const int size)
 * 機能				: 文字列を入力する（最大文字数のチェックあり）
 * 入力パラメータ	: const int size	:入力できる文字数（文字数チェックしないときは0）
 * 出力パラメータ	: char *s	:文字列
 * 戻り値			: int		:入力された文字数
 * 特記事項			: 100文字（半角）を超える部分は無視する
 */
int GetStr(char *s, const int size)
{
	char buf[GETSTR_BUF_SIZE];				/* 文字列入力バッファ */
	int len;								/* 入力した文字列の長さ */
	
	while(TRUE) {
		/* キーボードからの入力を文字列として取得する */
		fflush(stdin);						/* 入力バッファをクリア */
		fgets(buf, sizeof(buf), stdin);		/* 文字を入力 */
		len = strlen(buf);
		buf[--len] = '\0';					/* 改行コードを消す */
		fflush(stdin);						/* 入力を超えた分の文字をクリア */
		
		/* 文字列をチェックする */
		if((size > 0) && (len > size)) {	/* 指定文字数を超過 */
			printf("%d文字（半角）以内で入力してください。\n", size);
			printf("入力＞　");
		} else {
			strncpy_s(s, len+1, buf, _TRUNCATE);	/* sに文字列をセット */
			break;
		}
	}
	return len;
}

/*
 * 関数名			: int GetNumStr(char *s, const int size)
 * 機能				: 指定桁数（1～16桁）の数字のみの文字列を入力する
 * 入力パラメータ	: const int size	:桁数
 * 出力パラメータ	: char *s			:文字列
 * 戻り値			: int	:TRUE（指定桁数を入力）/FALSE（0,1のみ入力）
 * 特記事項			: 数字以外を入力した場合には再入力を促す
 */
int GetNumStr(char *s, const int size)
{
	int retVal = TRUE;
	char buf[GETNUMSTR_BUF_SIZE];		/* 文字列入力バッファ */
	int len;							/* 入力した文字列の長さ */
	
	while(TRUE) {
		/* キーボードからの入力を文字列として取得する */
		fflush(stdin);						/* 入力バッファをクリア */
		fgets(buf, sizeof(buf), stdin);		/* 文字を入力 */
		len = strlen(buf);
		buf[--len] = '\0';					/* 改行コードを消す */
		fflush(stdin);						/* 入力を超えた分の文字をクリア */
		
		/* 文字列をチェックする */
		if(len == 0) {									/* エンターキーのみ */
			s[0] = '\0';
			retVal = FALSE;
			break;
		} else if((len == 1) && (buf[0] == '1')) {		/* 1を入力 */
			strncpy_s(s, size+1, buf, _TRUNCATE);
			retVal = FALSE;
			break;
		} else if((len == 1) && (buf[0] == '0')) {		/* 0を入力 */
			strncpy_s(s, size+1, buf, _TRUNCATE);
			retVal = FALSE;
			break;
		} else if(len != size) {						/* 指定桁数以外 */
			printf("%d桁で入力してください。\n", size);
			printf("入力＞　");
		} else if(IsNumStr(buf, len) == FALSE) {		/* 数字以外の文字がある */
			printf("数字を入力してください。\n");
			printf("入力＞　");
		} else {
			strncpy_s(s, size+1, buf, _TRUNCATE);
			break;
		}
	}
	return retVal;
}

/*
 * 関数名			: int GetIntStr(char *s, const int s1, const int s2)
 * 機能				: 指定値1以上指定値2以下の整数を入力する
 * 入力パラメータ	: const int s1	:最小値
 * 					: const int s2	:最大値
 * 					:				 ただし、ともに0を指定した場合には値の範囲チェックはしない
 * 出力パラメータ	: char *s	:文字列
 * 戻り値			: int	:TRUE（正常入力）/FALSE（空文字(Enterのみ)入力）
 * 特記事項			: 数字以外を入力した場合には再入力を促す
 */
int GetIntStr(char *s, const int s1, const int s2)
{
	int retVal = TRUE;
	char buf[GETINTSTR_BUF_SIZE];		/* 文字列入力バッファ */
	int len;							/* 入力した文字列の長さ */
	int val;							/* 文字列を数値に変換した値 */
	
	while(TRUE) {
		/* キーボードからの入力を文字列として取得する */
		fflush(stdin);						/* 入力バッファをクリア */
		fgets(buf, sizeof(buf), stdin);		/* 文字を入力 */
		len = strlen(buf);
		buf[--len] = '\0';					/* 改行コードを消す */
		fflush(stdin);						/* 入力を超えた分の文字をクリア */
		
		/* 文字列をチェックする */
		if(len != 0) {
			if(IsNumStr(buf, len) == FALSE) {				/* 数字以外の文字がある ? */
				printf("数字を入力してください。\n");
				printf("入力＞　");
			} else {
				val = atoi(buf);
				if((s1 == 0) && (s2 == 0)) {				/* 数値の範囲チェックをしない */
					strncpy_s(s, len+1, buf, _TRUNCATE);	/* sに文字列をセット */
					retVal = TRUE;
					break;
				} else if((val >= s1) && (val <= s2)) {		/* 入力が範囲内 */
					strncpy_s(s, len+1, buf, _TRUNCATE);	/* sに文字列をセット */
					retVal = TRUE;
					break;
				} else {
					printf("数値が範囲外です。\n");
					printf("入力＞　");
				}
			}
		} else {
			s[0] = '\0';
			retVal = FALSE;									/* 入力なしの場合 */
			break;
		}
	}
	return retVal;
}

/*
 * 関数名			: void TrimSpace(char s[])
 * 機能				: 文字列の末尾の空白を削除する
 * 入力パラメータ	: char *s	:空白の入った文字列
 * 出力パラメータ	: char *s	:空白を除いた文字列
 * 戻り値			: なし
 * 特記事項			: 文字列の末尾から削除する
 */
void TrimSpace(char s[])
{
	int i;
	
	i = strlen(s) - 1;
	while(i >= 0) {
		if(s[i] == ' ') {
			s[i] = '\0';
			i--;
		} else {
			break;
		}
	}
}

/*
 * 関数名			: int GetFigure(int num)
 * 機能				: 整数の桁数を取得する
 * 入力パラメータ	: int num	:整数
 * 出力パラメータ	: なし
 * 戻り値			: int	:桁数
 * 特記事項			: なし
 */
int GetFigure(int num)
{
    int i = 1;
	
	num = num / 10;
	while(num != 0) {
		num = num / 10;
		i++;
	}
    return i;
}

/*
 * 関数名			: void InputYMD(char *date)
 * 機能				: 日付を年月日ごとに入力し、誤りがないかチェックする
 * 入力パラメータ	: なし
 * 出力パラメータ	: char *date	:日付
 * 戻り値			: なし
 * 特記事項			: なし
 */
void InputYMD(char *date)
{
	char sy[INPUTYMD_YEAR_BUF_SIZE];		/* 年 */
	char sm[INPUTYMD_MONTH_BUF_SIZE];		/* 月 */
	char sd[INPUTYMD_DAY_BUF_SIZE];			/* 日 */
	char buf[INPUTYMD_BUF_SIZE];			/* 8桁の日付 */
	
	while(TRUE) {
		/* 年を入力する */
		printf("年（西暦）＞　");
		GetIntStr(sy, MIN_YEAR, MAX_YEAR);
		
		/* 月を入力する */
		printf("月　　　　＞　");
		GetIntStr(sm, MIN_MONTH, MAX_MONTH);
		
		/* 日を入力する */
		printf("日　　　　＞　");
		GetIntStr(sd, MIN_DAY, MAX_DAY);
		
		/* 日付をチェックする */
		sprintf_s(buf, INPUTYMD_BUF_SIZE, "%04d%02d%02d", atoi(sy), atoi(sm), atoi(sd));
		if(IsDateStr(buf) == FALSE) {
			printf("日付の入力に誤りがあります。年（西暦）から再度入力してください。\n");
		} else {
			strncpy_s(date, DATE_STR_SIZE+1, buf, _TRUNCATE);
			break;
		}
	}
}

/*
 * 関数名			: int GetDateStr(char *date)
 * 機能				: 日付（YYYYMMDD）を入力する
 * 入力パラメータ	: なし
 * 出力パラメータ	: char *date	:文字列
 * 戻り値			: int	:TRUE（正常入力）/FALSE（空文字(Enterのみ)入力）
 * 特記事項			: 日付以外の数字を入力した場合には再入力を促す
 */
int GetDateStr(char *date)
{
	int retVal = TRUE;
	char buf[GETDATESTR_BUF_SIZE];		/* 文字列入力バッファ */
	int len;							/* 入力した文字列の長さ */
	
	while(TRUE) {
		/* キーボードからの入力を文字列として取得する */
		fflush(stdin);						/* 入力バッファをクリア */
		fgets(buf, sizeof(buf), stdin);		/* 文字を入力 */
		len = strlen(buf);
		buf[--len] = '\0';					/* 改行コードを消す */
		fflush(stdin);						/* 入力を超えた分の文字をクリア */
		
		/* 文字列をチェックする */
		if(len != 0) {
			if(IsNumStr(buf, len) == FALSE) {					/* 数字以外かチェック */
				printf("数字を入力してください。\n");
				printf("入力＞　");
			} else if(IsDateStr(buf) == FALSE) {				/* 日付の範囲かチェック */
				printf("日付が範囲外です。\n");
				printf("入力＞　");
			} else {
				strncpy_s(date, DATE_STR_SIZE+1, buf, _TRUNCATE);	/* dateに文字列をセット */
				break;
			}
		} else {
			date[0] = '\0';
			retVal = FALSE;										/* 入力なしの場合 */
			break;
		}
	}
	return retVal;
}

/*
 * 関数名			: void GetToday(const int y, const int m, const int d, char *date)
 * 機能				: 現在の日付に指定した日数を加減算した文字列を取得する
 * 入力パラメータ	: const int y	:加減算する年
 * 					: const int m	:加減算する月
 * 					: const int d	:加減算する日
 * 出力パラメータ	: char *date	:日付（YYYYMMDD）
 * 戻り値			: なし
 * 特記事項			: なし
 */
void GetToday(const int y, const int m, const int d, char *date)
{
	time_t tNow, tCal;
	struct tm tmDate;
	
	/* 現在の日付を取得する */
	tNow = time(NULL);
	localtime_s(&tmDate, &tNow);
	
	/* 指定した日数を加算した日付を取得する */
	tmDate.tm_year += y;			/* 年を加算 */
	tmDate.tm_mon += m;				/* 月を加算 */
	tmDate.tm_mday += d;			/* 日を加算 */
	tCal = mktime(&tmDate);			/* 日付を整形 */
	localtime_s(&tmDate, &tCal);	/* 表示用に年月日を分割 */
	
	/* 日付を年月日別に文字列として取得する */
	sprintf_s(date, DATE_STR_SIZE+1, "%04d%02d%02d", tmDate.tm_year + BASE_YEAR, tmDate.tm_mon + BASE_MONTH, tmDate.tm_mday);
}

/*
 * 関数名			: int IsDateStr(const char *date)
 * 機能				: 日付をチェックする
 * 入力パラメータ	: const char *date	:YYYYMMDD形式の文字列
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE/FALSE
 * 特記事項			: 1970年未満は無効とする
 */
int IsDateStr(const char *date)
{
	int retVal;
	const int dayTbl[] = {	/* 各月の月末日 */
		31,						/* 1月の月末日 */
		29,						/* 2月の月末日 （閏年を含む）*/
		31,						/* 3月の月末日 */
		30,						/* 4月の月末日 */
		31,						/* 5月の月末日 */
		30,						/* 6月の月末日 */
		31,						/* 7月の月末日 */
		31,						/* 8月の月末日 */
		30,						/* 9月の月末日 */
		31,						/* 10月の月末日 */
		30,						/* 11月の月末日 */
		31						/* 12月の月末日 */
	};
	char sy[ISDATESTR_YEAR_BUF_SIZE];		/* 年 */
	char sm[ISDATESTR_MONTH_BUF_SIZE];		/* 月 */
	char sd[ISDATESTR_DAY_BUF_SIZE];		/* 日 */
	int iy, im, id;
	
	if(strlen(date) != DATE_STR_SIZE) {						/* 文字数をチェック */
		retVal = FALSE;
	} else {
		strncpy_s(sy, ISDATESTR_YEAR_BUF_SIZE, date, YEAR_SIZE);
		strncpy_s(sm, ISDATESTR_MONTH_BUF_SIZE, date+YEAR_SIZE, MONTH_SIZE);
		strncpy_s(sd, ISDATESTR_DAY_BUF_SIZE, date+YEAR_SIZE+MONTH_SIZE, DAY_SIZE);
		iy = atoi(sy);
		im = atoi(sm);
		id = atoi(sd);
		if((im < MIN_MONTH) || (im > MAX_MONTH)){				/* 月数の範囲をチェック */
			retVal = FALSE;
		} else if((id < MIN_DAY) || (id > MAX_DAY)) {			/* 日数の範囲をチェック */
			retVal = FALSE;
		} else if(id > dayTbl[im-1]) {							/* 月日の範囲をチェック */
			retVal = FALSE;
		} else {
			retVal = TRUE;
		}
		if((iy % 4) != 0) {								/* 閏年をチェック 年が4で割り切れないとき */
			if((im == 2) && (id == 29)) {				/* 閏年でない時 2月29日はエラー */
				retVal = FALSE;
			}
		}
	}
	return retVal;
}

/*
 * 関数名			: void ConvFormatDate(char *dest, const char *src)
 * 機能				: 日付文字列にスラッシュを挿入する
 * 入力パラメータ	: const char *src	:入力文字列
 * 出力パラメータ	: char *dest		:出力文字列
 * 戻り値			: なし
 * 特記事項			: なし
 */
void ConvFormatDate(char *dest, const char *src)
{
	char y[CONVFORMATDATE_YEAR_BUF_SIZE];			/* 年 */
	char m[CONVFORMATDATE_MONTH_BUF_SIZE];			/* 月 */
	char d[CONVFORMATDATE_DAY_BUF_SIZE];			/* 日 */
	
	strncpy_s(y, CONVFORMATDATE_YEAR_BUF_SIZE, src, YEAR_SIZE);
	strncpy_s(m, CONVFORMATDATE_MONTH_BUF_SIZE, src+YEAR_SIZE, MONTH_SIZE);
	strncpy_s(d, CONVFORMATDATE_DAY_BUF_SIZE, src+YEAR_SIZE+MONTH_SIZE, DAY_SIZE);
	sprintf_s(dest, YEAR_SIZE+MONTH_SIZE+DAY_SIZE+3, "%s/%s/%s", y, m, d);
}

/*
 * 関数名			: int DiffDate(const char *fromdate,const char *toDate)
 * 機能				: ftomdate と toDate の二つの日付間隔を日数で返す
 * 入力パラメータ	: char *fromdate:日付（YYYYMMDD）文字列
 * 					: char *tomdate :日付（YYYYMMDD）文字列
 * 出力パラメータ	: なし
 * 戻り値			: int			:2つの日付間隔の日数 、パラメータエラーの時は常に 0
 * 特記事項			: fromDate > toDate の場合は負の値を返す
 *					: 1970年未満は無効とする
 */
int DiffDate(const char *fromDate,const char *toDate)
{
	struct tm tmFrom,tmTo;					/* 入力パラメータのカレンダー値格納用 tm時刻構造体 */
	char sYear[DIFFDATE_YEAR_BUF_SIZE];		/* 年 */
	char sMonth[DIFFDATE_MONTH_BUF_SIZE];	/* 月 */
	char sDay[DIFFDATE_DAY_BUF_SIZE];		/* 日 */
	int DateInterval;						/* 日数での間隔格納用 */

	if(IsDateStr(fromDate) && IsDateStr(toDate)){			/* 入力パラメータの確認 */
		/* fromDateをtm構造体に格納 */
		strncpy_s(sYear, DIFFDATE_YEAR_BUF_SIZE, fromDate, YEAR_SIZE);						/* 年を取り出し */
		strncpy_s(sMonth, DIFFDATE_MONTH_BUF_SIZE, fromDate+YEAR_SIZE, MONTH_SIZE);			/* 月を取り出し */
		strncpy_s(sDay, DIFFDATE_DAY_BUF_SIZE, fromDate+YEAR_SIZE+MONTH_SIZE, DAY_SIZE);	/* 日を取り出し */
		tmFrom.tm_year = atoi(sYear) - BASE_YEAR;				/* 年をセット */
		tmFrom.tm_mon = atoi(sMonth) - BASE_MONTH;				/* 月をセット */
		tmFrom.tm_mday = atoi(sDay);							/* 日をセット */
		tmFrom.tm_hour = 0;										/* 時に0をセット */
		tmFrom.tm_min = 0;										/* 分に0をセット */
		tmFrom.tm_sec = 0;										/* 秒に0をセット */
		tmFrom.tm_isdst = 0;									/* 夏時間は使用しない*/
		/* toDateをtm構造体に格納 */
		strncpy_s(sYear, YEAR_SIZE+1, toDate, YEAR_SIZE);						/* 年を取り出し */
		strncpy_s(sMonth, MONTH_SIZE+1, toDate+YEAR_SIZE, MONTH_SIZE);			/* 月を取り出し */
		strncpy_s(sDay, DAY_SIZE+1, toDate+YEAR_SIZE+MONTH_SIZE, DAY_SIZE);	/* 日を取り出し */
		tmTo.tm_year = atoi(sYear) - BASE_YEAR;					/* 年をセット */
		tmTo.tm_mon = atoi(sMonth) - BASE_MONTH;				/* 月をセット */
		tmTo.tm_mday = atoi(sDay);								/* 日をセット */
		tmTo.tm_hour = 0;										/* 時に0をセット */
		tmTo.tm_min = 0;										/* 分に0をセット */
		tmTo.tm_sec = 0;										/* 秒に0をセット */
		tmTo.tm_isdst = 0;										/* 夏時間は使用しない*/
		DateInterval =  (int)(mktime(&tmTo) - mktime(&tmFrom)) / (SECOND_OF_DAY);	/* 秒数から日数へ変換 */
	} else {
		DateInterval = 0;										/* パラメータエラーの時は常に 0 を返す */
	}
	return DateInterval;
}

/*
 * 関数名			: static int IsNumStr(const char *s, const int size)
 * 機能				: 文字が数字のみかチェックする
 * 入力パラメータ	: const char *s		:文字列
 * 					: const int size	:桁数
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE/FALSE
 * 特記事項			: なし
 */
static int IsNumStr(const char *s, const int size)
{
	int retVal = TRUE;
	int i;
	
	for(i=0; i<size; i++) {
		if((s[i] < '0') || (s[i] > '9')) {
			retVal = FALSE;
			break;
		}
	}
	return retVal;
}

/*
 * 関数名			: int GetMenuNum(const MENU *m)
 * 機能				: メニューを表示し、選択したメニュー番号を返す
 * 入力パラメータ	: const MENU *m	：MENU型構造体変数へのアドレス
 * 出力パラメータ	: なし
 * 戻り値			: int	:選択番号
 * 特記事項			: メニューにない番号を入力した場合には再入力を促す
 */
int GetMenuNum(const MENU *m)
{
	int i, in;
	char buf[GETMENUNUM_BUF_SIZE+STRING_TERMINATOR_SIZE];	/* メニュー番号入力バッファ */
	
	/* 選択番号を取得する */
	do {
		DisplayMenu(m);							/* メニュー画面を表示 */
		/* 選択番号を入力する */
		GetNumStr(buf, GETMENUNUM_BUF_SIZE);	/* 番号を文字列として取得 */
		if(buf[0] == '\0') {					/* 入力がエンターキーのみの場合 */
			in = NULL_NUM;						/* NULL番号（空文字）を設定 */
		} else {								/* ID入力がある場合 */
			in = atoi(buf);						/* 数値に変換 */
			for(i = 0; i < MENU_NUM; i++) {
				/* 入力番号が選択番号と等しいならばfor文を抜ける */
				if(m->sel[i].num == in) {
					break;
				}
			}
			/* 入力番号が選択番号と等しいならばwhile文を抜ける */
			if(i == MENU_NUM) {
				printf("選択番号が異なります。\n");
			}
		}
	} while((i == NULL_NUM) || (i == MENU_NUM));
	
	return in;									/* 入力番号を返す */
}

/*
 * 関数名			: void DisplayMenu(const MENU *m)
 * 機能				: メニューを表示する
 * 入力パラメータ	: const MENU *m	：MENU型構造体変数へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void DisplayMenu(const MENU *m)
{
	int i;
	
	printf("%s\n", m->title);
	for(i = 0; (i < MENU_NUM) && (m->sel[i].str != NULL); i++) {
		printf("%d:%s\n", m->sel[i].num, m->sel[i].str);
	}
	printf("入力＞　");
}

/*
 * 関数名			: void DisplayTitle(const TITLE *t)
 * 機能				: タイトルを表示する
 * 入力パラメータ	: const TITLE *t	：TITLE型構造体変数へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void DisplayTitle(const TITLE *t)
{
	printf("\n%s", t->title);
}

/*
 * 関数名			: char *ConvLendStr(const char *s)
 * 機能				: 貸出状況の表示文字例（貸出可/貸出中）に変換する
 * 入力パラメータ	: const char *s	:0/1の文字列
 * 出力パラメータ	: なし
 * 戻り値			: char *dsp		:貸出状況の文字列（貸出可/貸出中）
 * 特記事項			: なし
 */
char *ConvLendStr(const char *s)
{
	static const char *tbl[] = {GOODS_LEND_NO_NAME, GOODS_LEND_YES_NAME};
	char *dsp;
	
	if((*s == '0') || (*s == '1')) {
		dsp = (char *)tbl[atoi(s)];
	} else {
		dsp = "＿＿＿";
	}
	return dsp;
}

/*
 * 関数名			: char *ConvRetStr(const char *s)
 * 機能				: 返却状況の表示文字例（返却可/返却済）に変換する
 * 入力パラメータ	: const char *s	:0/1の文字列
 * 出力パラメータ	: なし
 * 戻り値			: char *dsp		:返却状況の文字列（返却可/返却済）
 * 特記事項			: なし
 */
char *ConvRetStr(const char *s)
{
	static const char *tbl[] = {GOODS_RET_NO_NAME, GOODS_RET_YES_NAME};
	char *dsp;
	
	if((*s == '0') || (*s == '1')) {
		dsp = (char *)tbl[atoi(s)];
	} else {
		dsp = "＿＿＿";
	}
	return dsp;
}

/*
 * 関数名			: char *ConvKeepStr(const char *s)
 * 機能				: 保管場所の表示文字例（倉庫/店頭）に変換する
 * 入力パラメータ	: const char *s	:0/1の文字列
 * 出力パラメータ	: なし
 * 戻り値			: char *dsp		:保管場所の文字列（倉庫/店頭）
 * 特記事項			: なし
 */
char *ConvKeepStr(const char *s)
{
	static const char *tbl[] = {GOODS_KEEP_WARE_NAME, GOODS_KEEP_SHOP_NAME};
	char *dsp;
	
	if((*s == '0') || (*s == '1')) {
		dsp = (char *)tbl[atoi(s)];
	} else {
		dsp = "＿＿";
	}
	return dsp;
}
