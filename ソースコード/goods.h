/*
 * ファイル名	: goods.h
 * 機能			: 商品管理を行う
 * 特記事項		: なし
 * 修正履歴		:  1  2012/01/10 V01L1 初版
 *				:  2  
 */

#ifndef GOODS_H
#define GOODS_H

/* ヘッダーの取り込み */
#include "member.h"

/* #defineマクロ */
/* バッファサイズ */
/* WriteGoodsFile */
#define WRITEGOODSFILE_BUF_SIZE				100		/* フォーマット用文字列格納用バッファサイズ */

/* DisplayGoods */
#define DISPLAYGOODS_BUF_SIZE				16		/* 日付フォーマット用文字列格納用バッファサイズ */
	
/* InputRegistGoods */
#define INPUTREGISTGOODS_BUF_SIZE			128		/* 文字列用バッファサイズ */
#define INPUTREGISTGOODS_FORMAT_BUF_SIZE	8		/* sprintfフォーマット用文字列用バッファサイズ */

/* GetGoodsID */
#define GETGOODSID_STR_BUF_SIZE				20		/* 文字列操作用バッファサイズ */
#define GETGOODSID_DATE_BUF_SIZE			10		/* 本日の日付文字列格納用バッファサイズ */

/* DisplayRegistGoods */
#define DISPLAYREGISTGOODS_BUF_SIZE			16		/* 日付フォーマット用文字列格納用バッファサイズ */
	
/* InputUpdateGoods */
#define INPUTUPDATEGOODS_DATE_BUF_SIZE		16		/* 日付フォーマット用文字列格納用バッファサイズ */
#define INPUTUPDATEGOODS_NUMSTR_BUF_SIZE	8		/* 貸出回数用文字列格納用バッファサイズ */

/* DisplayGoodsList */
#define DISPLAYGOODSLIST_HEAD_COL_NUM				30	/* DisplayGoodsList ヘッダー項目列数 */
#define DISPLAYGOODSLIST_HEAD_COL_BUF_SIZE			20	/* DisplayGoodsList ヘッダー項目文字列用バッファサイズ */
#define DISPLAYGOODSLIST_HEAD_BUF_SIZE				200	/* DisplayGoodsList ヘッダー文字列用バッファサイズ */
#define DISPLAYGOODSLIST_HEAD_FORMAT_COL_NUM		10	/* DisplayGoodsList printfのフォーマット文字列用列数 */
#define DISPLAYGOODSLIST_HEAD_FORMAT_COL_BUF_SIZE	10	/* DisplayGoodsList printfのフォーマット文字列用バッファサイズ */
#define DISPLAYGOODSLIST_DATE_BUF_SIZE				20	/* DisplayGoodsList 日付フォーマット用文字列用バッファサイズ */
#define DISPLAYGOODSLIST_DSP_BUF_SIZE				40	/* DisplayGoodsList 表示文字列の長さ調整用バッファサイズ */

/* 商品一覧画面表示 */
#define GOODS_MON_DSP_NUM		20					/* 1画面の商品表示数 */

/* 商品一覧表示：商品情報の表示桁数 */
#define GOODS_DSP_ID_NUM			13				/* 商品ID */
#define GOODS_DSP_TITLE_NUM			14				/* タイトル */
#define GOODS_DSP_GENRE_NUM			8				/* ジャンル */
#define GOODS_DSP_SELL_DATE_NUM		10				/* 発売日 */
#define GOODS_DSP_RENT_DATE_NUM		10				/* 貸出・返却日 */
#define GOODS_DSP_LEND_STS_NUM		6				/* 貸出状況 */
#define GOODS_DSP_KEEP_NUM			4				/* 保管場所 */

/* 商品一覧表示：項目表示列数 */
#define GOODS_DSP_COL_NUM				8			/* 表示項目数 */

/* 商品一覧表示：項目表示列番号 */
#define GOODS_DSP_ROWNO_COL			0				/* 行番号 */
#define GOODS_DSP_ID_COL			1				/* 商品ID */
#define GOODS_DSP_TITLE_COL			2				/* タイトル */
#define GOODS_DSP_GENRE_COL			3				/* ジャンル */
#define GOODS_DSP_SELL_DATE_COL		4				/* 発売日 */
#define GOODS_DSP_RENT_DATE_COL		5				/* 貸出・返却日 */
#define GOODS_DSP_LEND_STS_COL		6				/* 貸出状況 */
#define GOODS_DSP_KEEP_COL			7				/* 保管場所 */
	
/* 商品一覧表示：表示項目名（全角で5文字以内） */
#define GOODS_DSP_ROWNO_NAME		"No."			/* 行番号 */
#define GOODS_DSP_ID_NAME			"商品ID"		/* 商品ID */
#define GOODS_DSP_TITLE_NAME		"タイトル"		/* タイトル */
#define GOODS_DSP_GENRE_NAME		"ジャンル"		/* ジャンル */
#define GOODS_DSP_SELL_DATE_NAME	"発売日"		/* 発売日 */
#define GOODS_DSP_RENT_DATE_NAME	"貸出・返却日"	/* 貸出・返却日 */
#define GOODS_DSP_LEND_STS_NAME		"貸出状況"		/* 貸出状況 */
#define GOODS_DSP_KEEP_NAME			"保管場所"		/* 保管場所 */

/* 商品情報ファイル */
#define GOODS_FILE_NAME			"goodsMST.dat"		/* 商品情報ファイル名 */
#define GOODS_REG_MAX			10000				/* 商品最大登録数 */
#define GOODS_NUMBER_MAX		99999				/* 貸出回数の最大値 */

/* 商品情報ファイル：項目名（全角で5文字以内） */
#define GOODS_ID_NAME			"商品ID"			/* 商品ID */
#define GOODS_TITLE_NAME		"タイトル"			/* タイトル */
#define GOODS_GENRE_NAME		"ジャンル"			/* ジャンル */
#define GOODS_SELL_NAME			"発売日"			/* 発売日 */
#define GOODS_RENT_DATE_NAME	"貸出・返却日"		/* 貸出・返却日 */
#define GOODS_LEND_MID_NAME		"貸出会員ID"		/* 貸出会員ID */
#define GOODS_LEND_STS_NAME		"貸出状況"			/* 貸出状況 */
#define GOODS_LEND_NUM_NAME		"貸出回数"			/* 貸出回数 */
#define GOODS_KEEP_NAME			"保管場所"			/* 保管場所 */

/* 商品情報ファイル：商品情報の各桁数（構造体メンバーの大きさ） */
#define GOODS_ID_SIZE			13					/* 商品ID */
#define GOODS_TITLE_SIZE		20					/* タイトル */
#define GOODS_GENRE_SIZE		10					/* ジャンル */
#define GOODS_SELL_DATE_SIZE	8					/* 発売日 */
#define GOODS_RENT_DATE_SIZE	8					/* 貸出・返却日 */
#define GOODS_LEND_MID_SIZE		12					/* 貸出会員ID */
#define GOODS_LEND_STS_SIZE		1					/* 貸出状況 */
#define GOODS_LEND_NUM_SIZE		5					/* 貸出回数 */
#define GOODS_KEEP_SIZE			1					/* 保管場所 */

/* 商品情報ファイル：商品情報レコード内の先頭位置 */
#define GOODS_ID_POS			0											/* 商品ID */
#define GOODS_TITLE_POS			GOODS_ID_POS + GOODS_ID_SIZE				/* タイトル */
#define GOODS_GENRE_POS			GOODS_TITLE_POS + GOODS_TITLE_SIZE			/* ジャンル */
#define GOODS_SELL_DATE_POS		GOODS_GENRE_POS + GOODS_GENRE_SIZE			/* 発売日 */
#define GOODS_RENT_DATE_POS		GOODS_SELL_DATE_POS + GOODS_SELL_DATE_SIZE	/* 貸出・返却日 */
#define GOODS_LEND_MID_POS		GOODS_RENT_DATE_POS + GOODS_RENT_DATE_SIZE	/* 貸出会員ID */
#define GOODS_LEND_STS_POS		GOODS_LEND_MID_POS + GOODS_LEND_MID_SIZE	/* 貸出状況 */
#define GOODS_LEND_NUM_POS		GOODS_LEND_STS_POS + GOODS_LEND_STS_SIZE	/* 貸出回数 */
#define GOODS_KEEP_POS			GOODS_LEND_NUM_POS + GOODS_LEND_NUM_SIZE	/* 保管場所 */

/* 商品情報ファイル：商品情報レコードのサイズ */
#define GOODS_SIZE				GOODS_KEEP_POS + GOODS_KEEP_SIZE

/* structタグ定義 */
/* 商品情報 */
typedef struct goods {
	char id[GOODS_ID_SIZE+1];				/* 商品ID */
	char title[GOODS_TITLE_SIZE+1];			/* タイトル */
	char genre[GOODS_GENRE_SIZE+1];			/* ジャンル */
	char sellDat[GOODS_SELL_DATE_SIZE+1];	/* 発売日 */
	char rentDat[GOODS_RENT_DATE_SIZE+1];	/* 貸出・返却日 */
	char lendMID[MEMBER_ID_SIZE+1];			/* 貸出会員ID */
	char lendSts[GOODS_LEND_STS_SIZE+1];	/* 貸出状況 */
	char lendNum[GOODS_LEND_NUM_SIZE+1];	/* 貸出回数 */
	char keep[GOODS_KEEP_SIZE+1];			/* 保管場所 */
	struct goods *next;						/* 自己参照ポインタ */
}GOODS;

/* 関数プロトタイプ宣言 */
extern int ReadGoodsFile(GOODS **goods);
extern void FreeGoodsList(void);
extern int WriteGoodsFile(void);
extern int SearchGoods(const char *id, GOODS **g);
extern void ClearGoodsBuffer(GOODS *g);
extern int InputRegistGoods(GOODS *g);
extern void DisplayRegistGoods(const GOODS *g);
extern int RegistGoods(const GOODS *g);
extern void DisplayGoods(const GOODS *g);
extern int DisplayGoodsList(const int key, const int ud, const int menuNum);
extern int InputUpdateGoods(GOODS *g, GOODS *gBuf);
extern int DeleteGoods(const char *id);

#endif		/* end GOODS_H */

