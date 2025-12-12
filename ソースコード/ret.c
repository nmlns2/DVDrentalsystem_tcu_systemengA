/*
 * ファイル名	: ret.c
 * 機能			: 返却管理を行う
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  2012/02/14 V02L1
 */

/* ヘッダーの取り込み */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "goods.h"
#include "ret.h"
#include "menu.h"

/* static変数宣言 */
static RET *root = NULL;			/* 返却情報リストの先頭ノード参照用 */

/* static関数宣言 */
static void SortRetList();
static char *ConvDelayStr(const int d);

/*
 * 関数名			: void CreateRetList(void)
 * 機能				: 【D-1112】返却情報リストを初期化する
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void CreateRetList(void)
{
	/* ダミーノードを作成する */
	if((root = (RET*)malloc(sizeof(RET))) == NULL) {
		printf("動的メモリの取得に失敗しました。\n");
		exit(EXIT_FAILURE);
	}
	root->next = NULL;
}

/*
 * 関数名			: void ClearRetBuffer(RET *ret)
 * 機能				: 【D-1112】返却情報バッファをクリアする
 * 入力パラメータ	: RET *ret	:返却情報へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void ClearRetBuffer(RET *ret)
{
	ret->id[0] = '\0';
	ret->title[0] = '\0';
	ret->delay = 0;
	ret->lendSts[0] = '\0';
	ret->next = NULL;
}

/*
 * 関数名			: int DisplayRet(const GOODS *goods, RET *ret)
 * 機能				: 【D-1112】商品の返却情報を表示する
 * 入力パラメータ	: GOODS *goods	:商品情報ノードへのアドレス
 * 出力パラメータ	: LEND *ret	:返却情報へのアドレス
 * 戻り値			: int	:TRUE（商品情報を取得）/FALSE（商品IDがない，返却済み）
 * 特記事項			: なし
 */
int DisplayRet(const GOODS *goods, RET *ret)
{
	int retVal = TRUE;
	char baseDate[DISPLAYRET_DATE_BUF_SIZE];			/* 今日の日付 */
	
	if(goods != NULL) {
		/* 延滞を判断する */
		GetToday(0, 0, -RET_DLY_DAY, baseDate);					/* 延滞日数計算の基準日を取得  */
		ret->delay = DiffDate(goods->rentDat, baseDate);		/* 延滞日数計算 */
		
		/* 商品情報を返却情報にコピーする */
		strncpy_s(ret->id, GOODS_ID_SIZE+1, goods->id, _TRUNCATE);
		strncpy_s(ret->title, MEMBER_NAME_SIZE+1, goods->title, _TRUNCATE);
		strncpy_s(ret->lendSts, GOODS_LEND_STS_SIZE+1, goods->lendSts, _TRUNCATE);

		/* 返却情報を表示する */
		printf("\n*****返却商品情報確認*****\n");
		printf("\n%s:%s:%s:%s", ret->id, ret->title, ConvDelayStr(ret->delay), ConvRetStr(ret->lendSts));
		if(ret->delay>0){
			printf(" %d日 延滞料金 %d円", ret->delay,(RET_DLY_PRICE * ret->delay));
		}
		printf("\n");
		if(strncmp(ret->lendSts, "0", GOODS_LEND_STS_SIZE) == 0) {			/* 返却済み ? */
			printf("（注）この商品は返却済みです。\n");
			retVal = FALSE;
		}
	} else {
		retVal = FALSE;
	}
	return retVal;
}

/*
 * 関数名			: int RegistRet(const RET *ret)
 * 機能				: 【D-1112】返却情報を返却リストに登録する
 * 入力パラメータ	: RET *ret	:返却情報へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（登録可）/FALSE（登録不可）
 * 特記事項			: なし
 */
int RegistRet(const RET *ret)
{
	int retVal = FALSE;
	RET *newone;		/* 返却情報の追加ノード参照用 */
	RET *target;		/* ターゲットノード用 */
	int cnt;
	
	/* 新たなノードを追加する */
	if((newone = (RET*)malloc(sizeof(RET))) == NULL) {		/* ノードを作成 */
		printf("動的メモリの取得に失敗しました。\n");
		exit(EXIT_FAILURE);
	}
	
	/* 返却情報をセットする */
	strncpy_s(newone->id, GOODS_ID_SIZE+1, ret->id, _TRUNCATE);
	strncpy_s(newone->title, MEMBER_NAME_SIZE+1, ret->title, _TRUNCATE);
	strncpy_s(newone->lendSts, GOODS_LEND_STS_SIZE+1, "", _TRUNCATE);
	newone->delay = ret->delay;

	/* リストの末尾にノードを追加する */
	target = root;											/* ターゲットを先頭に設定 */
	cnt = 1;												/* カウンタを1に初期化 */
	while(target->next != NULL) {
		target = target->next;								/* ターゲットを更新して次のノードへ */
		cnt++;
	}
	
	if(cnt > RET_MAX) {
		printf("\n一度に%d枚以上を返却することはできません。\n", RET_MAX);
		retVal = FALSE;
	} else {
		newone->next = NULL;
		target->next = newone;
		printf("\n返却リストに%d枚目を追加しました。\n", cnt);
		retVal = TRUE;
	}
	return retVal;
}

/*
 * 関数名			: void FreeRetList(void)
 * 機能				: 【D-1112】返却情報リストをメモリから解放する
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void FreeRetList(void)
{
	RET *temp;
	
	while(root != NULL) {
		temp = root;
		root = root->next;
		free(temp);
	}
}

/*
 * 関数名			: int DisplayRetList(void)
 * 機能				: 【D-1114】返却商品リスト確認画面
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（登録可）/FALSE（登録不可）
 * 特記事項			: なし
 */
int DisplayRetList(void)
{
	int retVal = FALSE;
	RET *target;				/* ターゲットノード用 */
	int price;
	int cnt;
	
	target = root;									/* ターゲットを先頭に設定 */
	price = 0;										/* 延滞額を0に初期化 */
	cnt = 0;										/* カウンタを0に初期化 */
	printf("\n*****返却商品リスト確認*****\n");
	if(target != NULL) {
		while(target->next != NULL) {
			target = target->next;					/* ターゲットを更新して次のノードへ */
			cnt++;
			
			/* 延滞があるかないかを判断して料金を計算する */
			if(target->delay > 0) {
				price = price + (RET_DLY_PRICE * target->delay);
			}
			
			printf("%02d.%s:%-20s:%s", cnt, target->id, target->title, ConvDelayStr(target->delay));
			if(target->delay>0){
				printf(" %3d日 延滞料金 %5d円", target->delay, (RET_DLY_PRICE * target->delay));
			}
			printf("\n");
		}
		printf("\n延滞料金合計:%d円\n", price);
		retVal = TRUE;
	} else {
		retVal = FALSE;
	}
	return retVal;
}

/*
 * 関数名			: int UpdateGoodsRetSts(void)
 * 機能				: 【D-1114】商品情報ファイルの貸出・返却日、貸出状況を更新する（マッチング）
 * 入力パラメータ	: GOODS *goods	:商品情報リスト
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（更新）/FALSE（更新不可）
 * 特記事項			: なし
 */
int UpdateGoodsRetSts(GOODS *goods)
{
	int retVal = FALSE;
	RET *ret;										/* ターゲットノード用 */
	char today[UPDATEGOODSRETSTS_DATE_BUF_SIZE];	/* 本日の日付格納用 */
	
	GetToday(0, 0, 0, today);						/* 本日の日付を取得 */
	SortRetList();									/* 返却リストをID順にソート */
	ret = root;										/* ターゲットを先頭に設定 */
	
	/* 商品情報ファイルの貸出状況を更新する */
	if((ret != NULL) && (goods != NULL)) {
		/* 商品IDでデータのマッチングを行う */
		while((ret->next != NULL) && (goods->next != NULL)) {
			/* 商品IDがマッチングしたら商品情報リストを更新する */
			if(strncmp(ret->next->id, goods->next->id, GOODS_ID_SIZE) < 0) {
				ret = ret->next;					/* 返却リストのノードを更新 */
			} else if(strncmp(ret->next->id, goods->next->id, GOODS_ID_SIZE) > 0) {
				goods = goods->next;				/* 商品リストのノードを更新 */
			} else {
				/* 商品情報リストの貸出状況を「貸出可」に設定する */
				strncpy_s(goods->next->lendSts, GOODS_LEND_STS_SIZE+1, GOODS_LEND_NO_VAL, _TRUNCATE);
				/* 商品情報リストの貸出・返却日を本日の日付に設定する */
				strncpy_s(goods->next->rentDat, GOODS_RENT_DATE_SIZE+1, today, _TRUNCATE);
				
				ret = ret->next;					/* 返却リストのノードを更新 */
				goods = goods->next;				/* 商品リストのノードを更新 */
			}
		}
		WriteGoodsFile();							/* 商品情報リストのファイル書き込み */
		retVal = TRUE;
	} else {
		retVal = FALSE;
	}
	return retVal;
}

/*
 * 関数名			: static void SortRetList(void)
 * 機能				: 【D-1114】返却リストをID順にソートする
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
static void SortRetList(void)
{
	RET *sort;			/* ソート後の先頭ノード */
	RET *sTarget;		/* ソート用ターゲットノード */
	RET *rTarget;		/* ルート用ターゲットノード */
	RET *newone;		/* 商品情報の追加ノード参照用 */
	RET *temp;			/* メモリ解放用 */
	int cmp;			/* strncmpの戻り値用 */
	
	/* ソートリスト用のダミーノードを作成する */
	if((sort = (RET*)malloc(sizeof(RET))) == NULL) {
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
		if((newone = (RET*)malloc(sizeof(RET))) == NULL) {
			printf("動的メモリの取得に失敗しました。\n");
			exit(EXIT_FAILURE);
		}
		strncpy_s(newone->id, GOODS_ID_SIZE+1, rTarget->id, _TRUNCATE);
		strncpy_s(newone->title, MEMBER_NAME_SIZE+1, rTarget->title, _TRUNCATE);
		strncpy_s(newone->lendSts, GOODS_LEND_STS_SIZE+1, rTarget->lendSts, _TRUNCATE);
		newone->delay = rTarget->delay;

		/* ノードをソートリストに挿入する */
		sTarget = sort;
		while(TRUE) {
			/* 次のノードがあるならば、IDをチェックする */
			if(sTarget->next != NULL) {
				cmp = strncmp(newone->id, sTarget->next->id, GOODS_ID_SIZE);
				/* 追加ノードのIDが小さいならば、ターゲットの後ろにノードを追加して終了する */
				if(cmp < 0) {
					newone->next = sTarget->next;
					sTarget->next = newone;
					break;
				} else {
					sTarget = sTarget->next;
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
 * 関数名			: static char *ConvDelayStr(const int d)
 * 機能				: 延滞の表示文字例に変換する
 * 入力パラメータ	: const int d: 延滞日数
 * 出力パラメータ	: なし
 * 戻り値			: char *dsp		:延滞状態の文字
 * 特記事項			: なし
 */
static char *ConvDelayStr(const int d)
{
	char *dsp;
	
	if(d > 0) {
		dsp = RET_DLY_YES_NAME;
	} else {
		dsp = RET_DLY_NO_NAME;
	}
	return dsp;
}

