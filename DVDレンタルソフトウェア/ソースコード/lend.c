/*
 * ファイル名	: lend.c
 * 機能			: 貸出管理を行う
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  2012/02/14 V02L1
 */

/* ヘッダーの取り込み */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"
#include "goods.h"
#include "lend.h"
#include "menu.h"

/* static変数宣言 */
static LEND *root = NULL;		/* 貸出情報リストの先頭ノード参照用 */

/* static関数宣言 */
static void SortLendList(void);
static char *ConvOldStr(const char *s);
static int recommend_counter = 0;
static int ceo_index = -1;

/*
 * 関数名			: void CreateLendList(void)
 * 機能				: 【D-1112】貸出情報リストを初期化する
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void CreateLendList(void)
{
	/* ダミーノードを作成する */
	if((root = (LEND*)malloc(sizeof(LEND))) == NULL) {
		printf("動的メモリの取得に失敗しました。\n");
		exit(EXIT_FAILURE);
	}
	root->next = NULL;
}

/*
 * 関数名			: void ClearLendBuffer(LEND *lend)
 * 機能				: 【D-1112】貸出情報バッファをクリアする
 * 入力パラメータ	: LEND *lend	:貸出情報へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: CreateLendList を呼出後、FreeLendLsitを呼出すまでに使用すること
 */
void ClearLendBuffer(LEND *lend)
{
	lend->id[0] = '\0';
	lend->title[0] = '\0';
	lend->old[0] = '\0';
	lend->lendSts[0] = '\0';
	lend->next = NULL;
}

/*
 * 関数名			: int DisplayLend(const GOODS *goods, LEND *lend)
 * 機能				: 【D-1112】商品の貸出情報を表示する
 * 入力パラメータ	: GOODS *goods	:商品情報ノードへのアドレス
 * 出力パラメータ	: LEND *lend	:貸出情報へのアドレス
 * 戻り値			: int	:貸出可のときTRUE，貸出中のときFALSE
 * 特記事項			: CreateLendList を呼出後、FreeLendLsitを呼出すまでに使用すること
 */
int DisplayLend(const GOODS *goods, LEND *lend)
{
	int retVal = TRUE;
	char baseDate[DISPLAYLEND_DATE_BUF_SIZE];		/* 旧作と判断する日付 */
	
	if(goods != NULL) {
		/* 新作/旧作を判断する */
		GetToday(-LEND_OLD_YEAR, -LEND_OLD_MONTH, -LEND_OLD_DAY, baseDate);	/* 指定した日数前の日付を取得 */
		if(strncmp(goods->sellDat, baseDate, 8) < 0) {						/* 発売日と比較 */
			strncpy_s(lend->old, LEND_OLD_SIZE+1, LEND_OLD_VAL, _TRUNCATE);					/* 旧作 */
		} else {
			strncpy_s(lend->old, LEND_OLD_SIZE+1, LEND_NEW_VAL, _TRUNCATE);					/* 新作 */
		}
		
		/* 商品情報を貸出情報にコピーする */
		strncpy_s(lend->id, GOODS_ID_SIZE+1, goods->id, _TRUNCATE);
		strncpy_s(lend->title, MEMBER_NAME_SIZE+1, goods->title, _TRUNCATE);
		strncpy_s(lend->lendSts, GOODS_LEND_STS_SIZE+1, goods->lendSts, _TRUNCATE);

		/* 貸出情報を表示する */
		printf("\n*****貸出商品情報確認*****\n");
		printf("\n%s:%s:%s:%s\n", lend->id, lend->title, ConvOldStr(lend->old), ConvLendStr(lend->lendSts));
		if(strncmp(lend->lendSts, "1", GOODS_LEND_STS_SIZE) == 0) {			/* 貸出中 ? */
			printf("（注）この商品は貸出中です。\n");
			retVal = FALSE;
		}
	}
	return retVal;
}

/*
 * 関数名			: int RegistLend(const LEND *lend)
 * 機能				: 【D-1112】貸出情報を貸出リストに登録する
 * 入力パラメータ	: LEND *lend	:貸出情報へのポインタ
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（登録可）/FALSE（登録不可）
 * 特記事項			: CreateLendList を呼出後、FreeLendLsitを呼出すまでに使用すること
 */
int RegistLend(const LEND *lend)
{
	int retVal = FALSE;
	LEND *newone;				/* 貸出情報の追加ノード参照用 */
	LEND *target;				/* ターゲットノード用 */
	int cnt;
	
	/* 新たなノードを追加する */
	if((newone = (LEND*)malloc(sizeof(LEND))) == NULL) {			/* ノードを作成 */
		printf("動的メモリの取得に失敗しました。\n");
		exit(EXIT_FAILURE);
	}
	/* 貸出情報をセットする */
	strncpy_s(newone->id, GOODS_ID_SIZE+1, lend->id, _TRUNCATE);
	strncpy_s(newone->title, MEMBER_NAME_SIZE+1, lend->title, _TRUNCATE);
	strncpy_s(newone->old, LEND_OLD_SIZE+1, lend->old, _TRUNCATE);
	strncpy_s(newone->lendSts, GOODS_LEND_STS_SIZE+1, "", _TRUNCATE);

	/* リストの末尾にノードを追加する */
	target = root;									/* ターゲットを先頭に設定 */
	cnt = 1;
	while(target->next != NULL) {
		target = target->next;						/* ターゲットを更新して次のノードへ */
		cnt++;
	}
	
	if(cnt > LEND_MAX) {
		printf("\n一度に%d枚以上を貸し出すことはできません。\n", LEND_MAX);
		retVal = FALSE;
	} else {
		newone->next = NULL;
		target->next = newone;
		printf("\n貸出リストに%d枚目を追加しました。\n", cnt);
		retVal = TRUE;
	}
	return retVal;
}

/*
 * 関数名			: void FreeLendList(void)
 * 機能				: 【D-1112】貸出情報リストをメモリから解放する
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
void FreeLendList(void)
{
	LEND *temp;
	
	while(root != NULL) {
		temp = root;
		root = root->next;
		free(temp);
	}
}

/*
 * 関数名			: int DisplayLendList(void)
 * 機能				: 【D-1113】貸出商品リスト確認画面
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（貸出登録あり）/FALSE（貸出登録なし）
 * 特記事項			: なし
 */
int DisplayLendList(void)
{
	int retVal = FALSE;
	LEND *target;				/* ターゲットノード用 */
	int price;					/* 料金合計用 */
	int cnt;					/* カウンタ用 */
	
	target = root;						/* ターゲットを先頭に設定 */
	price = 0;							/* 料金合計を0に初期化 */
	cnt = 0;							/* カウンタを0に初期化 */
	printf("\n*****貸出商品リスト確認*****\n");
	if(target != NULL) {
		while(target->next != NULL) {
			target = target->next;		/* ターゲットを更新して次のノードへ */
			cnt++;
			
			/* 新作か旧作かを判断して料金を計算する */
			if(strncmp(target->old, LEND_NEW_VAL, LEND_OLD_SIZE) == 0) {
				price = price + LEND_NEW_PRICE;
			} else {
				price = price + LEND_OLD_PRICE;
			}
			
			printf("%02d.%s:%s:%s\n", cnt, target->id, target->title, ConvOldStr(target->old));
		}
		printf("\n料金合計:%d円\n", price);
		retVal = TRUE;
	} else {
		retVal = FALSE;
	}
	return retVal;
}

/*
 * 関数名			: int UpdateGoodsLendSts(const char *mID)
 * 機能				: 【D-1113】商品情報ファイルの貸出会員ID、貸出状況、貸出回数を更新する（マッチング）
 * 入力パラメータ	: const char *mID	:貸出会員ID / GOODS *goods	:商品情報リスト
 * 出力パラメータ	: なし
 * 戻り値			: int	:TRUE（更新）/FALSE（更新不可）
 * 特記事項			: なし
 */
int UpdateGoodsLendSts(const char *mID, GOODS *goods)
{
	int retVal = FALSE;
	LEND *lend;										/* ターゲットノード用 */
	char toDay[UPDATEGOODSLENDSTS_DATE_BUF_SIZE];	/* 貸出日の日付 */
	int num;										/* 貸出回数 */
	
	GetToday(0, 0, 0, toDay);						/* 本日の日付を取得 */
	SortLendList();									/* 貸出リストをID順にソート */
	lend = root;									/* ターゲットを先頭に設定 */
	
	/* 商品情報ファイルの貸出状況および貸出会員IDを更新する */
	if((lend != NULL) && (goods != NULL)) {
		/* 商品IDでデータのマッチングを行う */
		while((lend->next != NULL) && (goods->next != NULL)) {
			/* 商品IDがマッチングしたら商品情報リストを更新する */
			if(strncmp(lend->next->id, goods->next->id, GOODS_ID_SIZE) < 0) {
				lend = lend->next;						/* 貸出リストのノードを更新 */
			} else if(strncmp(lend->next->id, goods->next->id, GOODS_ID_SIZE) > 0) {
				goods = goods->next;					/* 商品リストのノードを更新 */
			} else {
				/* 商品情報リストの貸出会員IDを更新する */
				strncpy_s(goods->next->lendMID, GOODS_LEND_MID_SIZE+1, mID, _TRUNCATE);
				
				/* 商品情報リストの貸出日付を本日の日付に設定する */
				strncpy_s(goods->next->rentDat, GOODS_RENT_DATE_SIZE+1, toDay, _TRUNCATE);
				
				/* 商品情報リストの貸出状況を「貸出中」に設定する */
				strncpy_s(goods->next->lendSts, GOODS_LEND_STS_SIZE+1, GOODS_LEND_YES_VAL, _TRUNCATE);
				
				/* 商品情報リストの貸出回数を更新する */
				num = atoi(goods->next->lendNum);
				num++;
				if(GetFigure(num) > GOODS_LEND_NUM_SIZE) {	/* 貸出回数の桁数を取得 */
					printf("商品ID:%sの貸出回数が最大値を超えました。", goods->next->lendMID);
					num = 0;
				}
				sprintf_s(goods->next->lendNum, GOODS_LEND_NUM_SIZE+1, "%05d", num);
				
				lend = lend->next;							/* 貸出リストのノードを更新 */
				goods = goods->next;						/* 商品リストのノードを更新 */
			}
		}
		WriteGoodsFile();									/* 商品情報リストのファイル書き込み */
		retVal = TRUE;
	} else {
		retVal = FALSE;
	}
	return retVal;
}

/*
 * 関数名			: static void SortLendList(void)
 * 機能				: 【D-1113】貸出リストをID順にソートする
 * 入力パラメータ	: なし
 * 出力パラメータ	: なし
 * 戻り値			: なし
 * 特記事項			: なし
 */
static void SortLendList(void)
{
	LEND *sort;			/* ソート後の先頭ノード */
	LEND *sTarget;		/* ソート用ターゲットノード */
	LEND *rTarget;		/* ルート用ターゲットノード */
	LEND *newone;		/* 商品情報の追加ノード参照用 */
	LEND *temp;			/* メモリ解放用 */
	int cmp;			/* strncmpの戻り値用 */
	
	/* ソートリスト用のダミーノードを作成する */
	if((sort = (LEND*)malloc(sizeof(LEND))) == NULL) {
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
		if((newone = (LEND*)malloc(sizeof(LEND))) == NULL) {
			printf("動的メモリの取得に失敗しました。\n");
			exit(EXIT_FAILURE);
		}
		strncpy_s(newone->id, GOODS_ID_SIZE+1, rTarget->id, _TRUNCATE);
		strncpy_s(newone->title, MEMBER_NAME_SIZE+1, rTarget->title, _TRUNCATE);
		strncpy_s(newone->old, LEND_OLD_SIZE+1, rTarget->old, _TRUNCATE);
		strncpy_s(newone->lendSts, GOODS_LEND_STS_SIZE+1, rTarget->lendSts, _TRUNCATE);

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
 * 関数名			: static char *ConvOldStr(const char *s)
 * 機能				: 新作/旧作の表示文字例に変換する
 * 入力パラメータ	: const char *s	:0/1の文字列
 * 出力パラメータ	: なし
 * 戻り値			: char *dsp		:新作/旧作情報の文字列
 * 特記事項			: なし
 */
static char *ConvOldStr(const char *s)
{
	static const char *tbl[] = {LEND_NEW_NAME, LEND_OLD_NAME};
	char *dsp;
	
	if((*s == '0') || (*s == '1')) {
		dsp = (char *)tbl[atoi(s)];
	} else {
		dsp = "＿＿";
	}
	return dsp;
}
