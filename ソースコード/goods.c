/*
 * ファイル名	: goods.c
 * 機能			: 商品管理を行う
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  2012/02/14 V02L1
 */

/* ヘッダーの取り込み */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "menu.h"
#include "goods.h"

/* static変数宣言 */
static GOODS *root = NULL;		/* 商品情報リストの先頭ノード参照用 */

/* static関数宣言 */
static void CreateGoodsList(GOODS **goods);
static int GetGoodsID(char *id);
static void SortGoodsList(const int key, const int ud);

/*
 * 関数名			: int ReadGoodsFile(GOODS **goods)
 * 機能				: 商品情報を商品ID順にリスト構造に読み込む
 * 入力パラメータ	: なし
 * 出力パラメータ	: GOODS **goods	:読み込んだ商品情報の先頭ノードのアドレス
 * 戻り値			: int	:TRUE（読み込み正常）常にTRUEを返す
 * 特記事項			: 商品コードでソートしながら取り込む
 */
int ReadGoodsFile(GOODS **goods)
{
	int retVal = TRUE;
	GOODS *newone;								/* 商品情報の追加ノード参照用 */
	GOODS *target;								/* ターゲットノード用 */
	FILE* fp;									/* ファイルポインタ */
	char buf[GOODS_SIZE+REC_DELIMITER_SIZE];	/* ファイル読み込み領域（1レコードサイズより余裕を持たせる） */
	errno_t error;
	
	/* 商品情報ファイルを読み込む */
	error = fopen_s(&fp, GOODS_FILE_NAME, "r");
	if(error != 0) {
		printf("\n商品情報ファイルがありません。処理を終了します。\n");
		exit(EXIT_FAILURE);
	}
	
	/* ファイル読み込みのための前処理 */
	CreateGoodsList(&target);
	*goods = target;
	memset(buf, '\0', sizeof(buf));								/* ファイル読み込み領域をクリア */
	
	/* ファイルをEOFまで読み込む */
	while(fgets(buf, sizeof(buf), fp) != NULL) {				/* ファイル終了まで読み込む */
		
		/* 新たなノードを追加しデータを格納する */
		if((newone = (GOODS*)malloc(sizeof(GOODS))) == NULL) {	/* ノードを作成 */
			printf("動的メモリの取得に失敗しました。\n");
			exit(EXIT_FAILURE);
		}
		
		/* 商品情報をノードに格納する */
		strncpy_s(newone->id, GOODS_ID_SIZE+1, &buf[GOODS_ID_POS], _TRUNCATE);
		strncpy_s(newone->title, GOODS_TITLE_SIZE+1, &buf[GOODS_TITLE_POS], _TRUNCATE);
		strncpy_s(newone->genre, GOODS_GENRE_SIZE+1, &buf[GOODS_GENRE_POS], _TRUNCATE);
		strncpy_s(newone->sellDat, GOODS_SELL_DATE_SIZE+1, &buf[GOODS_SELL_DATE_POS], _TRUNCATE);
		strncpy_s(newone->rentDat, GOODS_RENT_DATE_SIZE+1, &buf[GOODS_RENT_DATE_POS], _TRUNCATE);
		strncpy_s(newone->lendMID, GOODS_LEND_MID_SIZE+1, &buf[GOODS_LEND_MID_POS], _TRUNCATE);
		strncpy_s(newone->lendSts, GOODS_LEND_STS_SIZE+1, &buf[GOODS_LEND_STS_POS], _TRUNCATE);
		strncpy_s(newone->lendNum, GOODS_LEND_NUM_SIZE+1, &buf[GOODS_LEND_NUM_POS], _TRUNCATE);
		strncpy_s(newone->keep, GOODS_KEEP_SIZE+1, &buf[GOODS_KEEP_POS], _TRUNCATE);
	

		/* 末尾の空白を削除する */
		TrimSpace(newone->id);
		TrimSpace(newone->title);
		TrimSpace(newone->genre);
		TrimSpace(newone->sellDat);
		TrimSpace(newone->rentDat);
		TrimSpace(newone->lendMID);
		TrimSpace(newone->lendSts);
		TrimSpace(newone->lendNum);
		TrimSpace(newone->keep);
		
		/* IDで昇順になるようにノードを追加する */
		target = root;											/* ターゲットを先頭に設定 */
		while(TRUE) {
			/* 次のノードがあるならば、IDをチェックする */
			if(target->next != NULL) {
				/* 追加ノードのIDが小さいならば、ターゲットの後ろにノードを追加して終了する */
				if(strncmp(newone->id, target->next->id, GOODS_ID_SIZE) < 0) {
					newone->next = target->next;
					target->next = newone;
					break;
				} else {
					target = target->next;						/* ターゲットを更新して次のノードへ */
				}
			} else {
				/* 新しいノードをリストの末尾に追加して終了する */
				newone->next = NULL;
				target->next = newone;
				break;
			}
		}
	}
	fclose(fp);
	return retVal;
}

/*
 * 関数名			: static void CreateGoodsList(GOODS **goods)
 * 機能				: 商品情報リストを初期化する
 * 入力パラメータ	: なし
 * 出力パラメータ	: GOODS **goods	:ダミーノードのアドレス
 * 戻り値			: なし
 * 特記事項			: なし
 */
static void CreateGoodsList(GOODS **goods)
{
	/* ダミーノードを作成する */
	FreeGoodsList();
	if((root = (GOODS*)malloc(sizeof(GOODS))) == NULL) {
		printf("動的メモリの取得に失敗しました。\n");
		exit(EXIT_FAILURE);
	}
	root->next = NULL;
	*goods = root;
}

/*
 * 関数名			: void FreeGoodsList(void)
 * 機能				: 商品情報リストをメモリから解放する
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void FreeGoodsList(void)
{
	GOODS *temp;
	
	while(root != NULL) {
		temp = root;
		root = root->next;
		free(temp);
	}
}

/*
 * 関数名			: int WriteGoodsFile(void)
 * 機能				: 商品情報リストをファイルに書き込む
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（書き込み正常）常にTRUEを返す
 * 特記事項			: 商品コードでソートしながら取り込む
 */
int WriteGoodsFile(void)
{
	int retVal = TRUE;
	FILE* fp;							/* ファイルポインタ */
	GOODS *target;						/* ターゲットノード用 */
	char form[WRITEGOODSFILE_BUF_SIZE];	/* フォーマット用文字列 */
	errno_t error;

	/* ファイルをオープンする */
	error = fopen_s(&fp, GOODS_FILE_NAME, "w");
	if(error != 0) {
		printf("\n商品情報ファイルに書き込みできません。処理を終了します。\n");
		exit(EXIT_FAILURE);
	}
	
	/* fprintfのフォーマットを設定する */
	sprintf_s(form, WRITEGOODSFILE_BUF_SIZE, "%%-%ds%%-%ds%%-%ds%%-%ds%%-%ds%%-%ds%%-%ds%%-%ds%%-%ds\n",
		GOODS_ID_SIZE,
		GOODS_TITLE_SIZE,
		GOODS_GENRE_SIZE,
		GOODS_SELL_DATE_SIZE,
		GOODS_RENT_DATE_SIZE,
		GOODS_LEND_MID_SIZE,
		GOODS_LEND_STS_SIZE,
		GOODS_LEND_NUM_SIZE,
		GOODS_KEEP_SIZE
	);
	
	/* 商品情報リストをファイルに書き込む */
	target = root;
	while(target->next != NULL) {
		/* リストの内容を書き込む */
		target = target->next;
		fprintf(fp, form,
			target->id,
			target->title,
			target->genre,
			target->sellDat,
			target->rentDat,
			target->lendMID,
			target->lendSts,
			target->lendNum,
			target->keep
		);
	}
	fclose(fp);
	return retVal;
}
 
/*
 * 関数名			: int SearchGoods(const char *id, GOODS **g)
 * 機能				: 【D-1111】指定したIDの商品情報を取得する
 * 入力パラメータ	: const char *id	:検索する商品ID
 * 出力パラメータ	: GOODS **g			:検索された商品情報ノードへのポインタ
 * 戻り値			: int	:TRUE（商品情報を取得）/FALSE（指定した商品IDがない）
 * 特記事項			: なし
 */
int SearchGoods(const char *id, GOODS **g)
{
	int retVal = FALSE;
	GOODS *target;			/* ターゲットノード用 */

	target = root;
	while(target->next != NULL) {
		target = target->next;
		if(strncmp(target->id, id, GOODS_ID_SIZE) == 0) {
			*g = target;		/* ヒットしたデータのアドレスを戻り値に設定 */
			retVal = TRUE;
			break;
		}
	}
	return retVal;
}

/*
 * 関数名			: void ClearGoodsBuffer(GOODS *g)
 * 機能				: 【D-1131】商品情報バッファをクリアする
 * 入力パラメータ	: GOODS *g	:商品情報へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void ClearGoodsBuffer(GOODS *g)
{
	g->id[0] = '\0';
	g->title[0] = '\0';
	g->genre[0] = '\0';
	g->sellDat[0] = '\0';
	g->rentDat[0] = '\0';
	g->lendMID[0] = '\0';
	g->lendSts[0] = '\0';
	g->lendNum[0] = '\0';
	g->keep[0] = '\0';
	g->next = NULL;
}

/*
 * 関数名			: int InputRegistGoods(GOODS *g)
 * 機能				: 【D-1131】登録する商品情報を入力する
 * 入力パラメータ	: なし
 * 出力パラメータ	: GOODS *g	:入力した商品情報へのポインタ
 * 戻り値			: int	:TRUE（商品ID取得）/FALSE（商品ID取得失敗）
 * 特記事項			: なし
 */
int InputRegistGoods(GOODS *g)
{
	int retVal;
	char buf[INPUTREGISTGOODS_BUF_SIZE];				/* 文字列用バッファ */
	char id[GOODS_ID_SIZE+STRING_TERMINATOR_SIZE];		/* 商品ID用文字列 */
	char f[INPUTREGISTGOODS_FORMAT_BUF_SIZE];			/* sprintfフォーマット用文字列 */
	
	/* タイトルを入力する */
	printf("%-12s＞　", GOODS_TITLE_NAME);
	buf[0] = '\0';
	while(strlen(buf) == 0) {
		GetStr(buf, GOODS_TITLE_SIZE);
	}
	strncpy_s(g->title, GOODS_TITLE_SIZE+1, buf, _TRUNCATE);
	
	/* ジャンルを入力する */
	printf("%-12s＞　", GOODS_GENRE_NAME);
	buf[0] = '\0';
	while(strlen(buf) == 0) {
		GetStr(buf, GOODS_GENRE_SIZE);
	}
	strncpy_s(g->genre, GOODS_GENRE_SIZE+1, buf, _TRUNCATE);
	
	/* 発売日を入力する */
	printf("%s\n", GOODS_SELL_NAME);
	buf[0] = '\0';
	InputYMD(buf);
	strncpy_s(g->sellDat, GOODS_SELL_DATE_SIZE+1, buf, _TRUNCATE);
	
	/* 貸出・返却日を新規設定する */
	sprintf_s(f, INPUTREGISTGOODS_FORMAT_BUF_SIZE, "%%0%dd", GOODS_RENT_DATE_SIZE);
	sprintf_s(g->rentDat, GOODS_RENT_DATE_SIZE+1, f, 0);
	
	/* 貸出状況を新規設定する */
	strncpy_s(g->lendSts, GOODS_LEND_STS_SIZE+1, GOODS_LEND_NO_VAL, _TRUNCATE);
	
	/* 貸出会員IDを新規設定する */
	sprintf_s(f, INPUTREGISTGOODS_FORMAT_BUF_SIZE, "%%0%dd", GOODS_LEND_MID_SIZE);
	sprintf_s(g->lendMID, MEMBER_ID_SIZE+1, f, 0);
	
	/* 貸出回数を新規設定する */
	sprintf_s(f, INPUTREGISTGOODS_FORMAT_BUF_SIZE, "%%0%dd", GOODS_LEND_NUM_SIZE);
	sprintf_s(g->lendNum, GOODS_LEND_NUM_SIZE+1, f, 0);
	
	/* 保管場所を新規設定する */
	strncpy_s(g->keep, GOODS_KEEP_SIZE+1, GOODS_KEEP_WARE_VAL, _TRUNCATE);
	
	/* 商品IDを新規設定する */
	if(GetGoodsID(id) == FALSE) {					/* IDの最大値を超えた場合 */
		printf("商品IDの取得に失敗しました。\n");
		retVal = FALSE;
	} else {										/* IDを取得できた場合 */
		strncpy_s(g->id, GOODS_ID_SIZE+1, id, _TRUNCATE);
		retVal = TRUE;
	}
	return retVal;
}

/*
 * 関数名			: static int GetGoodsID(char *id)
 * 機能				:  【D-1131】新規の商品IDを取得する
 * 入力パラメータ	: なし
 * 出力パラメータ	: char *id	:新規に取得した商品ID
 * 戻り値			: int	:TRUE（商品IDを取得）/FALSE（商品IDを取得できない）
 * 特記事項			: "99"+日付＋3桁の数字　計13桁
 */
static int GetGoodsID(char *id)
{
	int retVal = FALSE;
	GOODS *target;							/* ターゲットノード用 */
	char w[GETGOODSID_STR_BUF_SIZE];		/* 文字列操作用 */
	char today[GETGOODSID_DATE_BUF_SIZE];	/* 本日の日付 */
	int newID;								/* 新規ID（下位10桁の数字） */
	int listID;								/* 商品情報リスト内のID（下位10桁の数字） */
	
	/* 10桁の会員IDを作成する（INT型では10桁までしか取れないので先頭の'2'を削除する） */
	GetToday(0, 0, 0, today);					/* 今日の日付を取得 */
	strncpy_s(w, GETGOODSID_STR_BUF_SIZE, today + 1, 7);		/* 2012年の先頭の2を削除 */
	sprintf_s(w, GETGOODSID_STR_BUF_SIZE, "%07s%03d", w, 1);	/* 日付+"01"のIDを作成 */
	newID = atoi(w);
	
	/* 商品情報リストを検索し、新しいIDを取得する */
	target = root;
	if(target != NULL) {
		/* リストが終了するまで繰り返す */
		while(target->next != NULL) {
			target = target->next;
			
			/* リストの商品IDの下位10ケタを取得する（INT型では10桁まで有効） */
			strncpy_s(w, GETGOODSID_STR_BUF_SIZE, target->id + 3, 10);
			listID = atoi(w);
			
			/* IDを比較する */
			if(newID == listID) {
				newID = listID + 1;
			} else if(newID > listID) {
				/* DO NOTHING */
			} else {
				break;
			}
		}
		
		/* IDの最大値をチェックする */
		if((newID % 1000) == 0) {				/* 999を超えた */
			retVal = FALSE;
		} else {
			sprintf_s(id, GOODS_ID_SIZE+1, "992%010d", newID);	/* '2'を付加して文字列に変換 */
			retVal = TRUE;
		}
	} else {
		retVal = FALSE;
	}
	return retVal;
}

/*
 * 関数名			: void DisplayRegistGoods(const GOODS *g)
 * 機能				: 【D-1132】登録する商品情報を表示する
 * 入力パラメータ	: GOODS *g	:商品情報へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void DisplayRegistGoods(const GOODS *g)
{
	char date[DISPLAYREGISTGOODS_BUF_SIZE];					/* 日付フォーマット用 */
	
	printf("\n*****新規商品情報確認*****\n");
	printf("%-10s:%s\n", GOODS_ID_NAME, g->id);
	printf("%-10s:%s\n", GOODS_TITLE_NAME, g->title);
	printf("%-10s:%s\n", GOODS_GENRE_NAME, g->genre);
	ConvFormatDate(date, g->sellDat);
	printf("%-10s:%s\n", GOODS_SELL_NAME, date);
}

/*
 * 関数名			: int RegistGoods(const GOODS *g)
 * 機能				: 【D-1132】商品情報を商品リストに登録する
 * 入力パラメータ	: GOODS *g	:商品情報へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（商品情報を登録）常にTRUEを返す
 * 特記事項			: なし
 */
int RegistGoods(const GOODS *g)
{
	int retVal = FALSE;
	GOODS *newone;				/* 商品情報の追加ノード参照用 */
	GOODS *target;				/* ターゲットノード用 */
	
	/* 新たなノードを追加しデータを格納 */
	if((newone = (GOODS*)malloc(sizeof(GOODS))) == NULL) {		/* ノードを作成 */
		printf("動的メモリの取得に失敗しました。\n");
		exit(EXIT_FAILURE);
	}
	
	strncpy_s(newone->id, GOODS_ID_SIZE+1, g->id, _TRUNCATE);
	strncpy_s(newone->title, GOODS_TITLE_SIZE+1, g->title, _TRUNCATE);
	strncpy_s(newone->genre, GOODS_GENRE_SIZE+1, g->genre, _TRUNCATE);
	strncpy_s(newone->sellDat, GOODS_SELL_DATE_SIZE+1, g->sellDat, _TRUNCATE);
	strncpy_s(newone->rentDat, GOODS_RENT_DATE_SIZE+1, g->rentDat, _TRUNCATE);
	strncpy_s(newone->lendMID, GOODS_LEND_MID_SIZE+1, g->lendMID, _TRUNCATE);
	strncpy_s(newone->lendSts, GOODS_LEND_STS_SIZE+1, g->lendSts, _TRUNCATE);
	strncpy_s(newone->lendNum, GOODS_LEND_NUM_SIZE+1, g->lendNum, _TRUNCATE);
	strncpy_s(newone->keep, GOODS_KEEP_SIZE+1, g->keep, _TRUNCATE);
	
	/* IDで昇順になるようにノードを追加する */
	target = root;												/* ターゲットを先頭に設定 */
	while(TRUE) {
		/* 次のノードがあるならば、IDをチェックする */
		if(target->next != NULL) {
			/* 追加ノードのIDが小さいならば、ターゲットの後ろにノードを追加して終了する */
			if(strncmp(newone->id, target->next->id, GOODS_ID_SIZE) < 0) {
				newone->next = target->next;
				target->next = newone;
				break;
			} else {
				target = target->next;							/* ターゲットを更新して次のノードへ */
			}
		} else {
			/* 新しいノードをリストの末尾に追加して終了する */
			newone->next = NULL;
			target->next = newone;
			break;
		}
	}
	return TRUE;
}

/*
 * 関数名			: void DisplayGoods(const GOODS *g)
 * 機能				: 【D-1133】商品情報を表示する
 * 入力パラメータ	: GOODS *g	:商品情報ノードへのアドレス
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void DisplayGoods(const GOODS *g)
{
	char date[DISPLAYGOODS_BUF_SIZE];					/* 日付フォーマット用 */
	
	printf("\n*****商品情報確認*****\n");
	printf("%-11s:%s\n", GOODS_ID_NAME, g->id);
	printf("%-11s:%s\n", GOODS_TITLE_NAME, g->title);
	printf("%-11s:%s\n", GOODS_GENRE_NAME, g->genre);
	ConvFormatDate(date, g->sellDat);
	printf("%-11s:%s\n", GOODS_SELL_NAME, date);
	ConvFormatDate(date, g->rentDat);
	printf("%-11s:%s\n", GOODS_RENT_DATE_NAME, date);
	printf("%-11s:%s\n", GOODS_LEND_MID_NAME, g->lendMID);
	printf("%-11s:%s\n", GOODS_LEND_STS_NAME, ConvLendStr(g->lendSts));
	printf("%-11s:%d\n", GOODS_LEND_NUM_NAME, atoi(g->lendNum));
	printf("%-11s:%s\n", GOODS_KEEP_NAME, ConvKeepStr(g->keep));
	
}

/*
 * 関数名			: int DisplayGoodsList(const int key, const int ud, const int menuNum)
 * 機能				: 【D-1137】商品の一覧を表示する
 * 入力パラメータ	: const int key	:ソートキー
 * 					: const int ud	:昇順/降順（1:昇順/2:降順）
 * 					: const int num	:画面からの選択番号（ENTER_NUM:初回、以外:2回目以降）
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE(まだ表示あり) / FALSE(最後まで表示)
 * 特記事項			: なし
 */
int DisplayGoodsList(const int key, const int ud, const int menuNum)
{
	int retVal;
	static GOODS *target;	/* ターゲットノード用 */
	static int dspNo = 1;	/* 表示の項番 */
	
	char h[DISPLAYGOODSLIST_HEAD_COL_NUM][DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE];	/* ヘッダー項目 */
	char head[DISPLAYGOODSLIST_HEAD_BUF_SIZE];	/* ヘッダー */
	char f[DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM][DISPLAYGOODSLIST_HEAD_FORMAT_COL_BUF_SIZE];	/* printfのフォーマット用 */
	char date[DISPLAYGOODSLIST_DATE_BUF_SIZE];	/* 日付フォーマット用 */
	int fig;									/* 項番の桁数 */
	int cnt;									/* 表示カウンタ */
	char buf[DISPLAYGOODSLIST_DSP_BUF_SIZE];	/* 表示文字列の長さ調整用 */
	
	/* 最初の入力の場合はリストを先頭に戻し、指定キーで昇順/降順にソートする */
	if(menuNum == ENTER_NUM) {
		target = root;
		dspNo = 1;
		SortGoodsList(key, ud);
	}

	/* ヘッダーを表示する */
	fig = GetFigure(GOODS_REG_MAX);			/* 項番の桁数を取得 */
	printf("\n*****商品情報一覧*****\n");
	strncpy_s(h[GOODS_DSP_ROWNO_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_ROWNO_NAME, _TRUNCATE);
	
	/* （ここ以下は文字列の長さが変更される可能性があるため、strncpyを使っていない） */
	strcpy_s(h[GOODS_DSP_ID_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_ID_NAME);
	strcpy_s(h[GOODS_DSP_TITLE_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_TITLE_NAME);
	strcpy_s(h[GOODS_DSP_GENRE_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_GENRE_NAME);
	strcpy_s(h[GOODS_DSP_SELL_DATE_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_SELL_DATE_NAME);
	strcpy_s(h[GOODS_DSP_RENT_DATE_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_RENT_DATE_NAME);
	strcpy_s(h[GOODS_DSP_LEND_STS_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_LEND_STS_NAME);
	strcpy_s(h[GOODS_DSP_KEEP_COL], DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE, GOODS_DSP_KEEP_NAME);
	sprintf_s(head, DISPLAYGOODSLIST_HEAD_BUF_SIZE, "%%-%ds %%-%ds %%-%ds %%-%ds %%-%ds %%-%ds %%-%ds %%-%ds\n",
		fig,
		GOODS_DSP_ID_NUM,
		GOODS_DSP_TITLE_NUM,
		GOODS_DSP_GENRE_NUM,
		GOODS_DSP_SELL_DATE_NUM,
		GOODS_DSP_RENT_DATE_NUM,
		GOODS_DSP_LEND_STS_NUM,
		GOODS_DSP_KEEP_NUM
	);
	printf(head, 
		h[GOODS_DSP_ROWNO_COL],
		h[GOODS_DSP_ID_COL],
		h[GOODS_DSP_TITLE_COL],
		h[GOODS_DSP_GENRE_COL],
		h[GOODS_DSP_SELL_DATE_COL],
		h[GOODS_DSP_RENT_DATE_COL],
		h[GOODS_DSP_LEND_STS_COL],
		h[GOODS_DSP_KEEP_COL]
	);
		
	/* printfのフォーマットを設定する */
	sprintf_s(f[GOODS_DSP_ROWNO_COL],	DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%0%dd ", fig);
	sprintf_s(f[GOODS_DSP_ID_COL], DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%-%ds ", GOODS_DSP_ID_NUM);
	sprintf_s(f[GOODS_DSP_TITLE_COL], DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%-%ds ", GOODS_DSP_TITLE_NUM);
	sprintf_s(f[GOODS_DSP_GENRE_COL], DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%-%ds ", GOODS_DSP_GENRE_NUM);
	sprintf_s(f[GOODS_DSP_SELL_DATE_COL], DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%-%ds ", GOODS_DSP_SELL_DATE_NUM);
	sprintf_s(f[GOODS_DSP_RENT_DATE_COL], DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%-%ds ", GOODS_DSP_RENT_DATE_NUM);
	sprintf_s(f[GOODS_DSP_LEND_STS_COL], DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%-%ds ", GOODS_DSP_LEND_STS_NUM);
	sprintf_s(f[GOODS_DSP_KEEP_COL], DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM, "%%-%ds ", GOODS_DSP_KEEP_NUM);
	
	/* 商品リストを表示する */
	cnt = 0;
	while(target->next != NULL) {
		/* リストの内容を表示する */
		target = target->next;
		printf(f[GOODS_DSP_ROWNO_COL], dspNo++);
		printf(f[GOODS_DSP_ID_COL], target->id);
		strncpy_s(buf, DISPLAYGOODSLIST_DSP_BUF_SIZE, target->title, _TRUNCATE);
		printf(f[GOODS_DSP_TITLE_COL], buf);
		strncpy_s(buf, DISPLAYGOODSLIST_DSP_BUF_SIZE, target->genre, _TRUNCATE);
		printf(f[GOODS_DSP_GENRE_COL], buf);
		ConvFormatDate(date, target->sellDat);
		printf(f[GOODS_DSP_SELL_DATE_COL], date);
		ConvFormatDate(date, target->rentDat);
		printf(f[GOODS_DSP_RENT_DATE_COL], date);
		printf(f[GOODS_DSP_LEND_STS_COL], ConvLendStr(target->lendSts));
		printf(f[GOODS_DSP_KEEP_COL], ConvKeepStr(target->keep));
		printf("\n");
		/* 表示ページを制御する */
		cnt++;
		if(cnt == GOODS_MON_DSP_NUM) {
			break;
		}
	}
	
	if(target->next == NULL) {
		printf("\n最後まで表示しました。\n");
		retVal = FALSE;
	} else {
		retVal = TRUE;
	}
	return retVal;
}

/*
 * 関数名			: static void SortGoodsList(const int key, const int ud)
 * 機能				: 【D-1137】商品情報リストを指定キーで昇順/降順にソートする
 * 入力パラメータ	: const int key	:ソートキー
 * 					: const int ud	:昇順/降順（1:昇順/2:降順）
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
static void SortGoodsList(const int key, const int ud)
{
	GOODS *sort;		/* ソート後の先頭ノード */
	GOODS *sTarget;		/* ソート用ターゲットノード */
	GOODS *rTarget;		/* ルート用ターゲットノード */
	GOODS *newone;		/* 商品情報の追加ノード参照用 */
	GOODS *temp;		/* メモリ解放用 */
	int cmp;			/* strncmpの戻り値用 */
	
	/* ソートリスト用のダミーノードを作成する */
	if((sort = (GOODS*)malloc(sizeof(GOODS))) == NULL) {			/* ノードを作成 */
		printf("動的メモリの取得に失敗しました。\n");
		exit(EXIT_FAILURE);
	}
	sort->next = NULL;
	
	/* ターゲットをルートの先頭に設定する */
	rTarget = root;
	
	/* ソートリストを作成する */
	while(rTarget->next != NULL) {
		/* ルート側のノードをソート側に複写する */
		rTarget = rTarget->next;
		if((newone = (GOODS*)malloc(sizeof(GOODS))) == NULL) {		/* ノードを作成 */
			printf("動的メモリの取得に失敗しました。\n");
			exit(EXIT_FAILURE);
		}
		strncpy_s(newone->id, GOODS_ID_SIZE+1, rTarget->id, _TRUNCATE);
		strncpy_s(newone->title, GOODS_TITLE_SIZE+1, rTarget->title, _TRUNCATE);
		strncpy_s(newone->genre, GOODS_GENRE_SIZE+1, rTarget->genre, _TRUNCATE);
		strncpy_s(newone->sellDat, GOODS_SELL_DATE_SIZE+1, rTarget->sellDat, _TRUNCATE);
		strncpy_s(newone->rentDat, GOODS_RENT_DATE_SIZE+1, rTarget->rentDat, _TRUNCATE);
		strncpy_s(newone->lendMID, MEMBER_ID_SIZE+1, rTarget->lendMID, _TRUNCATE);
		strncpy_s(newone->lendSts, GOODS_LEND_STS_SIZE+1, rTarget->lendSts, _TRUNCATE);
		strncpy_s(newone->lendNum, GOODS_LEND_NUM_SIZE+1, rTarget->lendNum, _TRUNCATE);
		strncpy_s(newone->keep, GOODS_KEEP_SIZE+1, rTarget->keep, _TRUNCATE);
		
		/* ノードをソートリストに挿入する */
		sTarget = sort;
		while(TRUE) {
			/* 次のノードがあるならば、ソートキーをチェックする */
			if(sTarget->next != NULL) {
				/* ソートキーを選択する */
				switch(key) {
				case 1:				/* 1:商品ID */
					cmp = strncmp(newone->id, sTarget->next->id, GOODS_ID_SIZE);
					break;
				case 2:				/* 2:保管場所 */
					cmp = strncmp(newone->keep, sTarget->next->keep, GOODS_KEEP_SIZE);
					break;
				case 3:				/* 3:発売日 */
					cmp = strncmp(newone->sellDat, sTarget->next->sellDat, GOODS_SELL_DATE_SIZE);
					break;
				case 4:				/* 4:貸出・返却日 */
					cmp = strncmp(newone->rentDat, sTarget->next->rentDat, GOODS_RENT_DATE_SIZE);
					break;
				default:			/* 例外 */
					cmp = 0;
					break;
				}
				
				/* 追加ノードのソートキーの昇順、降順の判定*/
				if(ud == 1) {	/* 昇順にソート 追加ノードのソートキーの比較結果が小さいならば、ターゲットの後ろにノードを追加 */
					if(cmp < 0) {
						newone->next = sTarget->next;
						sTarget->next = newone;
						break;
					} else {
						sTarget = sTarget->next;
					}
				} else {		/* 降順にソート 追加ノードのソートキーの比較結果が大きいならば、ターゲットの後ろにノードを追加*/
					if(cmp > 0) {
						newone->next = sTarget->next;
						sTarget->next = newone;
						break;
					} else {
						sTarget = sTarget->next;
					}
				}
			} else {
				/* 新しいノードをリストの末尾に追加して終了する */
				newone->next = NULL;
				sTarget->next = newone;
				break;
			}
		}
	}
	
	/* ソートリストとルートリストのルートを入れ替える */
	temp = root->next;
	root->next = sort->next;
	sort->next = temp;
	
	/* ソートリストを解放する */
	while(sort != NULL) {
		temp = sort;
		sort = sort->next;
		free(temp);
	}
}

/*
 * 関数名			: int InputUpdateGoods(GOODS *g)
 * 機能				: 【D-1134】商品情報変更入力画面
 * 入力パラメータ	: GOODS *g	:変更する商品情報ノードへのポインタ
 * 出力パラメータ	: GOODS *gBuf	:変更した商品情報ノードへのポインタ
 * 戻り値			: int	:TRUE（商品情報を取得）/FALSE（商品IDがない）
 * 特記事項			: 既存の商品情報を参考に新たな商品情報を入力する
 */
int InputUpdateGoods(GOODS *g, GOODS *gBuf)
{
	int retVal = TRUE;
	char buf[GOODS_TITLE_SIZE+STRING_TERMINATOR_SIZE];	/* 文字列用バッファ */
	char date[INPUTUPDATEGOODS_DATE_BUF_SIZE];			/* 日付フォーマット用 */
	char num[INPUTUPDATEGOODS_NUMSTR_BUF_SIZE];			/* 貸出回数用 */
	
	if(g != NULL) {
		printf("変更項目は入力してください。\n");
		printf("未入力項目は変更しません。\n");
		printf("%s:%s\n", GOODS_ID_NAME, g->id);

		/* 商品IDをコピーする */
		strncpy_s(gBuf->id, GOODS_ID_SIZE+1, g->id, _TRUNCATE);
		
		/* タイトルを更新する */
		printf("%s:%s　＞　", GOODS_TITLE_NAME, g->title);
		GetStr(buf, GOODS_TITLE_SIZE);
		if(strlen(buf) != 0) {
			strncpy_s(gBuf->title, GOODS_TITLE_SIZE+1, buf, _TRUNCATE);
		} else {
			strncpy_s(gBuf->title, GOODS_TITLE_SIZE+1, g->title, _TRUNCATE);
		}
		
		/* ジャンルを更新する */
		printf("%s:%s　＞　", GOODS_GENRE_NAME, g->genre);
		GetStr(buf, GOODS_GENRE_SIZE);
		if(strlen(buf) != 0) {
			strncpy_s(gBuf->genre, GOODS_GENRE_SIZE+1, buf, _TRUNCATE);
		} else {
			strncpy_s(gBuf->genre, GOODS_GENRE_SIZE+1, g->genre, _TRUNCATE);
		}
		
		/* 発売日を更新する */
		ConvFormatDate(date, g->sellDat);
		printf("%s（'/'を除く8桁）:%s　＞　",GOODS_SELL_NAME, date);
		GetDateStr(buf);
		if(strlen(buf) != 0) {
			strncpy_s(gBuf->sellDat, GOODS_SELL_DATE_SIZE+1, buf, _TRUNCATE);
		} else {
			strncpy_s(gBuf->sellDat, GOODS_SELL_DATE_SIZE+1, g->sellDat, _TRUNCATE);
		}
		
		/* 貸出・返却日を更新する */
		ConvFormatDate(date, g->rentDat);
		printf("%s（'/'を除く8桁）:%s　＞　", GOODS_RENT_DATE_NAME, date);
		GetDateStr(buf);
		if(strlen(buf) != 0) {
			strncpy_s(gBuf->rentDat, GOODS_RENT_DATE_SIZE+1, buf, _TRUNCATE);
		} else {
			strncpy_s(gBuf->rentDat, GOODS_RENT_DATE_SIZE+1, g->rentDat, _TRUNCATE);
		}
		
		/* 貸出会員IDを更新する */
		printf("%s:%s　＞　", GOODS_LEND_MID_NAME, g->lendMID);
		GetNumStr(buf, GOODS_LEND_MID_SIZE);
		if(strlen(buf) != 0) {
			strncpy_s(gBuf->lendMID, GOODS_LEND_MID_SIZE+1, buf, _TRUNCATE);
		} else {
			strncpy_s(gBuf->lendMID, GOODS_LEND_MID_SIZE+1, g->lendMID, _TRUNCATE);
		}
		
		/* 貸出状況を更新する */
		printf("%s（1:貸出中、0;貸出可）:%s　＞　", GOODS_LEND_STS_NAME, ConvLendStr(g->lendSts));
		GetIntStr(buf, 1, 1);
		if(strlen(buf) != 0) {
			strncpy_s(gBuf->lendSts, GOODS_LEND_STS_SIZE+1, buf, _TRUNCATE);
		} else {
			strncpy_s(gBuf->lendSts, GOODS_LEND_STS_SIZE+1, g->lendSts, _TRUNCATE);
		}
		
		/* 貸出回数を更新する */
		printf("%s:%d　＞　", GOODS_LEND_NUM_NAME, atoi(g->lendNum));
		GetIntStr(buf, 0, GOODS_NUMBER_MAX);
		if(strlen(buf) != 0) {
			sprintf_s(num, INPUTUPDATEGOODS_NUMSTR_BUF_SIZE, "%05d", atoi(buf));			/* 頭を0で埋める */
			strncpy_s(gBuf->lendNum, GOODS_LEND_NUM_SIZE+1, num, _TRUNCATE);
		} else {
			strncpy_s(gBuf->lendNum, GOODS_LEND_NUM_SIZE+1, g->lendNum, _TRUNCATE);
		}

		/* 保管場所を更新する */
		printf("%s（1:店頭、0:倉庫）:%s　＞　", GOODS_KEEP_NAME, ConvKeepStr(g->keep));
		GetIntStr(buf, 1, 1);
		if(strlen(buf) != 0) {
			strncpy_s(gBuf->keep, GOODS_KEEP_SIZE+1, buf, _TRUNCATE);
		} else {
			strncpy_s(gBuf->keep, GOODS_KEEP_SIZE+1, g->keep, _TRUNCATE);
		}
	} else {
		retVal = FALSE;
	}
	return retVal;
}

/*
 * 関数名			: int DeleteGoods(const char *id)
 * 機能				: 【D-1138】商品情報削除通知画面
 * 入力パラメータ	: const char *id	:商品ID
 * 出力パラメータ	: なし
 * 戻り値			: int	:1（削除）/EXIT_NUM（削除しない）
 * 特記事項			: なし
 */
int DeleteGoods(const char *id)
{
	int retVal = EXIT_NUM;
	GOODS *target;					/* ターゲットノード用 */
	GOODS *del;
	
	printf("\n*****商品情報削除確認*****");
	if(GetYesNo("削除してもよろしいですか?") != FALSE) {
		/* リストからターゲットノードを削除する */
		target = root;
		while(target->next != NULL) {
			if(strncmp(target->next->id, id, GOODS_ID_SIZE) == 0) {
				del = target->next;
				target->next = target->next->next;
				free(del);
				break;
			}
			target = target->next;
		}
		WriteGoodsFile();						/* 商品情報リストのファイル書込み */
		printf("\n商品情報を削除しました。\n");
		retVal = 1;
	}
	return retVal;
}

