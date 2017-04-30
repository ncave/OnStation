
/*
	Loop closure subroutines
	Copyright (c) Jan 1995 J.P. Lebel / Calgary AB.
	Please forward any proposed upgrade suggestions to:
	Pierre Lebel (plebel@home.com)
	Taco VanIeperen (tacovan@smarttech.com)
*/ 

#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "fcntl.h"
#include "conio.h"
#include "jpclose.h"
#include "node.h"
#include "surveyshot.h"
#include "surveyleg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//####################################################################
//DEFINITIONS
CSurveyShot *little_shot_info, **LittleShots;

float deg_rad={3.141592632f/180.0f};
float pi={3.141592632f};

/* solver linear equation structure */ 
struct Lin_eq svv_big, svv_little;
unsigned long BigNumNodes;
unsigned long LittleNumNodes, BigConnections;
unsigned long *BigNodeNum_A, *LittleNodeNum_A;
unsigned long *BigNodeNum_B, *LittleNodeNum_B;
long *BigOrder;
CNode	**BigNodes;
CNode	**LittleNodes;


//####################################################################
/* invert square matrix from & to can be same */ 
int CClose::mat_inv
	( float *to
	, float *from
	, unsigned int size)
{
	float *work1,	temp;
	unsigned int	ia,	ix, iy, *flagr,*flagc, xp, yp;

	work1 = (float*) malloc(size * size * sizeof(float));
	flagr = (unsigned *) malloc(size * sizeof(int));
	flagc = (unsigned *) malloc(size * sizeof(int));
		
	/* transfer matrix into work space */ 
	for(iy=0 ; iy < size ; iy++){
		for(ix=0 ; ix < size ; ix++){
			work1[size*iy+ix] = from[size*iy+ix];
			to[size*iy+ix] = 0.0f;
		}	
		flagr[iy] = 1;
		flagc[iy] = 1;
		to[size*iy+iy] = 1.0f;
	}

	/* once for each row */ 
	for(ia=0 ; ia < size ; ia++){

		/* find largest remaining diagonal entry for pivot */ 
		temp = 0.0f;
		for(iy=0 ; iy < size ; iy++){
			if(flagr[iy] && flagc[iy]){
				if(fabs(work1[size*iy+iy]) > temp){
					xp = iy; yp = iy;temp = (float)fabs(work1[size*iy+iy]);
				}	
			}
		}

		if((xp >= size) | (yp >= size)){
			free(work1);
			free(flagr);
			free(flagc);
			return((int) 0); /* singular */ 
		}

		/* normalize pivot row & initialize pivot column in "to" array */ 
		temp = work1[size*yp+xp];
		for(ix=0 ; ix < size ; ix++){
			work1[size*yp+ix]	/= temp;
			to[size*yp+ix]		/= temp;
		}

		for(iy=0 ; iy < size ; iy++){
			if(iy != yp){
				temp = -work1[size*iy+xp];
				for(ix=0 ; ix < size ; ix++){
					if(flagc[ix]){
						work1[size*iy+ix]	+= temp * work1[size*yp+ix];
					}
					to[size*iy+ix]		+= temp * to[size*yp+ix];
				}
			}
		}
		flagc[xp] = 0;
		flagr[yp] = 0;
	}

	free(work1);
	free(flagr);
	free(flagc);
	return((int) 1);
}


//####################################################################
// cross multiply two (square) matrices: all three can be same matrix 
int CClose::mat_x_mat
	( float *to
	, float *from1
	, float *from2
	, unsigned int size)
{
	float *work1;
	unsigned int	ia,	ix, iy;
	work1 = (float*) malloc(size * size * sizeof(float));

	/* zero target array */ 
	for(iy=0 ; iy < size ; iy++){
		for(ix=0 ; ix < size ; ix++){
			work1[size*iy+ix] = 0.0f;
		}	
	}

	/* each column in target */ 
	for(ia=0 ; ia < size ; ia++){

		/* each element in target column */ 
		for(iy=0 ; iy < size ; iy++){

			/* each element in source */ 
			for(ix=0 ; ix < size ; ix++){
				work1[size*iy+ia] += from1[size*iy+ix] * from2[size*ix+ia];
			}	
		}
	}

	/* transfer results */ 
	for(iy=0 ; iy < size*size ; iy++){
		to[iy] = work1[iy];
	}

	free(work1);
	return((int) 0);
}


//####################################################################
// cross multiply square matrix and vector: "to" can be same as vect
int CClose::mat_x_vect
	( float *to
	, float *mat
	, float *vect
	, unsigned int size)	
{
	float *work1;
	unsigned int	ix, iy;
	work1 = (float*)		malloc(size * sizeof(float));

	/* zero target array */ 
	for(iy=0 ; iy < size ; iy++){
		work1[iy] = 0.0f;
	}

	/* each element in target column */ 
	for(iy=0 ; iy < size ; iy++){

		/* each element in source */ 
		for(ix=0 ; ix < size ; ix++){
			work1[iy] += mat[size*iy+ix] * vect[ix];
		}	
	}

	/* transfer results */ 
	for(iy=0 ; iy < size ; iy++){
		to[iy] = work1[iy];
	}

	free(work1);
	return((int) 1);
}
/* end mat_x_vect */ 



//####################################################################
// add two vectors: "to" can be same as either vect
// oper: 0 add both 1 sub first
//       2 sub 2nd  3 sub both
int CClose::vect_add
	( float *to
	, float *vect1
	, float *vect2
	, unsigned int size
	, unsigned int oper)	
{
	float *work1;
	unsigned int	iy;
	work1 = (float*)		malloc(size * sizeof(float));

	/*	target array */ 
	if((oper == 0) || (oper == 2))
	{
		for(iy=0 ; iy < size ; iy++)
		{
			work1[iy] = vect1[iy];
		}
	}
	else{
		for(iy=0 ; iy < size ; iy++){
			work1[iy] = -vect1[iy];
		}
	}
	if(oper < 2){
		for(iy=0 ; iy < size ; iy++){
			work1[iy] += vect2[iy];
		}
	}
	else{
		for(iy=0 ; iy < size ; iy++){
			work1[iy] -= vect2[iy];
		}
	}

	/* transfer results */ 
	for(iy=0 ; iy < size ; iy++){
		to[iy] = work1[iy];
	}

	free(work1);
	return((int) 1);
}


//####################################################################
// debug display (square) matrix
int CClose::mat_disp
	( float *from
	, TCHAR *name
	, unsigned int size) 
{
//	unsigned int	ix, iy;

	/* display array */ 
//		printf(_T("\n	ARRAY = %15s"),name);
//	for(iy=0 ; iy < size ; iy++){
//		printf(_T("\nrow %5i = "),iy);
//		for(ix=0 ; ix < size ; ix++) printf(_T("	%10.4f"),from[size*iy+ix]);
//}
//		printf(_T("\n\n hit any key"));
//	getch();

	return((int) 1);
}


//####################################################################
// scale and add submatrix
int CClose::pmat_insert(
	struct	Lin_eq *matrix,
	float *sub,
	unsigned long row,
	unsigned long col,
	float fact)
{
	float *values;
	unsigned long	r,	r1,	r2,	r3,	col3;
	unsigned		long	*c;

	/* Add new values */ 
	r1 = matrix->m_RI[row*3];
	r2 = matrix->m_RI[row*3+1];
	r3 = matrix->m_RI[row*3+2];
	c	= matrix->m_C + r1;
	col3 = col * 3;

	/* find correct first column */ 
	for(r=r2 ; (r1<r) && *c != col3 ; c++) {r1++; r2++; r3++;}

	values = matrix->m_A + r1;
	*values++ += fact * sub[0];
	*values++ += fact * sub[1];
	*values++ += fact * sub[2];

	values = matrix->m_A + r2;
	*values++ += fact * sub[1];
	*values++ += fact * sub[4];
	*values++ += fact * sub[5];

	values = matrix->m_A + r3;
	*values++ += fact * sub[2];
	*values++ += fact * sub[5];
	*values++ += fact * sub[8];

	if(row == col){
		r1 = row*3;
		r2 = r1+1;
		r3 = r2+1;
		matrix->m_D[r1] += (fact * sub[0]);
		matrix->m_D[r2] += (fact * sub[4]);
		matrix->m_D[r3] += (fact * sub[8]);
	}

	if(	(fabs(sub[1]-sub[3]) > .1f)
		|| (fabs(sub[6]-sub[2]) > .1f)
		|| (fabs(sub[7]-sub[5]) > .1f)
		|| (fabs(sub[0]) < 1e-10f)
		|| (fabs(sub[4]) < 1e-10f)
		|| (fabs(sub[8]) < 1e-10f) )
	{
		ASSERT(FALSE);
		TRACE(_T("Submatrix problem\n"));
		return FALSE;
//			printf(_T("\nWarning, matrix problem row=%li	col=%li"),row,col);
//			mat_disp(sub, _T("sub-array values"),3);
//		getch();
	}

	return(TRUE);
}


//####################################################################
// dump packed matrix to file
int CClose::pmat_dump(
	struct Lin_eq *matrix,
	unsigned long num	) 
{
/*
	float value;
	long	ic;
	unsigned		long	ix, iy, ia;
	FILE	*fil;

	fil = _tfopen(_T("OUTMATR.LDP"), _T("wa"));

	for (iy=0 ; iy<num * 3 ; iy++)
	{
			
		for(ix=0 ; ix < num * 3 ; ix++){
			value = 0.0f;
			ic = -1;
			for(ia=matrix->m_RI[iy] ; ia<matrix->m_RI[iy+1] && ic < 0 ; ia++)
			{
				if(matrix->m_C[ia] == ix) ic = ia;
			}
			if(ic >= 0) value = matrix->m_A[ic];
			fprintf(fil, _T("%12.6f"),value);
		}
		fprintf(fil, _T("\n"));
	}
	fclose(fil);
	*/ 
	return((int) 1);
}


//####################################################################
// dump packed vector to file
int CClose::vect_dump
	( float *from
	, unsigned long size)	
{
/*
	unsigned		long	iy;
	FILE	*fil;

	fil = _tfopen(_T("OUTVECT.LDP"), _T("wa"));

	for(iy=0 ; iy<size ; iy++) fprintf(fil, _T("%10.5f\n"),*from++);

	fclose(fil);
	*/ 
	return((int) 1);
}


//####################################################################
// find a good order for shots & nodes
long * CClose::good_order
	( CSurveyShot **BigShots
	, unsigned long NumBigShots
	, CNode **node_tbl)	
{

	ASSERT(AfxCheckMemory());	
	unsigned long
		BigLegNum,
		NumJunctions,
		count,
		templong, templong2, i1, i2,
		*useds,	*usedn,*usedn_tmp, NextNodeNum,
		*useds_tmp,	CurrentNodeNum, CurrentShotNum, NumStrings;


	unsigned long *NodeConnIndex, *JunctionNodeList, *NodeConnInfo;
	long	*order,	*ORDtmp;
	int		*known, *open, ok;

	CNode	**BigNodes;

	/* get good order to process BigShots */ 
	TRACE(_T("BM1\n"));
	ASSERT(AfxCheckMemory());
	BigNodes = node_tbl;

	useds	= (unsigned long*) malloc(NumBigShots * sizeof(long));
	useds_tmp = useds;

	TRACE(_T("BM1a\n"));
	for(count=0 ; count<NumBigShots ; count++ ) *(useds_tmp++) = 0;
	useds_tmp = useds;

	usedn	= (unsigned long*) malloc(BigNumNodes * sizeof(long));
	usedn_tmp = usedn;
	for(count=0 ; count<BigNumNodes ; count++ )	*(usedn_tmp++) = 0;
	usedn_tmp = usedn;
	
	known = (int*) malloc(BigNumNodes * sizeof(int));
	open  = (int*) malloc(BigNumNodes * sizeof(int));
	NodeConnIndex = (unsigned long*) malloc((BigNumNodes+1) * sizeof(long));



	// Scan shots
	// create table of number of string connections to each node
	NumJunctions = 0;
	NumStrings = 0;
	templong = 0;
	for(count=0 ; count<BigNumNodes ; count++ ){
		known[count] = (BigNodes[count])->m_Position.m_iNodeType;
		open[count]  = (BigNodes[count])->m_Position.m_iTotalOfAllShots;

		// build index vector too because the table address of each node must be stored
		NodeConnIndex[count] = templong;
		templong += open[count];
		if((open[count] != 2) || known[count] == 1) {
			NumJunctions++;
			NumStrings += open[count];
		}
	}
	NodeConnIndex[count] = templong;
	NumStrings /= 2;   // strings have 2 ends!!
	BigConnections = NumStrings;

	// build a table of node numbers which are junctions (conn != 2 or constrained)
	templong = 0;
	JunctionNodeList = (unsigned long*) malloc(NumJunctions * sizeof(long));
	for(count=0 ; count<BigNumNodes ; count++ ){
		if((open[count] != 2) || known[count] == 1){
			JunctionNodeList[templong++] = count;
		}
	}

	// build table of shot numbers which connect to each node
	NodeConnInfo = (unsigned long*) malloc(NodeConnIndex[BigNumNodes] * sizeof(long));
	for(BigLegNum = 0 ; BigLegNum < NumBigShots ; BigLegNum++){
		templong = BigNodeNum_A[BigLegNum];
		templong2 = NodeConnIndex[templong] + usedn[templong];
		usedn[templong] += 1;
		NodeConnInfo[templong2] = BigLegNum;
		
		templong = BigNodeNum_B[BigLegNum];
		templong2 = NodeConnIndex[templong] + usedn[templong];
		usedn[templong] += 1;
		NodeConnInfo[templong2] = BigLegNum;
	}

	usedn_tmp = usedn;
	for(count=0 ; count<BigNumNodes ; count++ )	*(usedn_tmp++) = 0;

	order	= (long*) malloc(NumBigShots * sizeof(long));
	ORDtmp = order;

	// scan junction node table - order shots from beginning of strings
	// strings don't have to be in any particular order
	templong = 0;
	for(count=0 ; count < NumJunctions ; count++ ){

		// try each leg that exits the junction
		for( i1 = NodeConnIndex[JunctionNodeList[count]]
		   ; i1 < NodeConnIndex[JunctionNodeList[count]+1]
		   ; i1++){
			CurrentNodeNum = JunctionNodeList[count]; // reset back to orig. junction
			CurrentShotNum = NodeConnInfo[i1];

			// test the first shot to see that we've not done it already
			if(useds[CurrentShotNum] == 0){

				// get ready to traverse string
				ok = 1;
				while(ok){

					// record shot as the next one in order
					order[templong++] = CurrentShotNum;
					useds[CurrentShotNum] = 1;

					// find the next node to go to
					if(BigNodeNum_A[CurrentShotNum] == CurrentNodeNum){
						NextNodeNum = BigNodeNum_B[CurrentShotNum];
					} else {
						NextNodeNum = BigNodeNum_A[CurrentShotNum];
					}
					
					// continue if the node isn't a junction
					if((open[NextNodeNum] == 2) && known[NextNodeNum] == 0){
						CurrentNodeNum = NextNodeNum;
						i2 = NodeConnIndex[CurrentNodeNum];
						if(NodeConnInfo[i2] == CurrentShotNum) {
							CurrentShotNum = NodeConnInfo[i2+1];
						} else {
							CurrentShotNum = NodeConnInfo[i2];
						}
					}	else {
						ok = 0;
					}
				}
			}
		}
	}



	TRACE(_T("BM4\n"));
	free(useds);
	free(usedn);
	free(known);
	free(open);
	free(NodeConnIndex);
	free(JunctionNodeList);
	free(NodeConnInfo);

	if(templong < NumBigShots){
		TRACE(_T("count<NumBigShots\n"));
		ASSERT(FALSE);
		free(order);
		return(FALSE);
	}
	//BigConnections = num_junct_conn / 2;
	TRACE(_T("BM5\n"));

	return(order);
}




//####################################################################
// build matrix to solve for station coordinates
int CClose::build_matrix
	( CSurveyShot **BigShots
	, unsigned long NumBigShots
	, CNode **node_tbl)	
{

	ASSERT(AfxCheckMemory());	
	unsigned long
		BigLegNum,	LittleLegNum,	count,
		BigNodeNumTmp_A, BigNodeNumTmp_B,
		LittleNodeNumTmp_A, LittleNodeNumTmp_B,
		ix,
		mode,
		*usedn,	*usedn_tmp, little_count,
		last_from,	ConFlag;

	CSurveyShot	*BigLegTmp, *LittleLegTmp;
	CNode	*BigNodeTmp_A, *BigNodeTmp_B, *BigNodeTmp_T;
	CNode	*LittleNodeTmp_A, *LittleNodeTmp_B, *LittleNodeTmp_T;

	float kmat[9], dp[3], LittleCmat[9], LittleRHS_Tmp[3], lmat[9];

	float
		len, thd, inc,
		vect[3],
		a, b, c, d, w1, w2, w3, asq, bsq, csq, dsq;

	/* solver storage pointers */ 
	float	**svv_xp, *svv_rhs;
	float	*svv_D, *svv_a;
	unsigned long *svv_c;
	unsigned long *svv_ri;

	/* solver linear equation pointers
	   - don't want to reference through structure all the time */ 
	svv_xp  = svv_big.m_ANS;
	svv_rhs = svv_big.m_B;
	svv_D   = svv_big.m_D;
	svv_a   = svv_big.m_A;
	svv_c   = svv_big.m_C;
	svv_ri  = svv_big.m_RI;

	usedn	= (unsigned long*) malloc(BigNumNodes * sizeof(long));
	usedn_tmp = usedn;
	for(count=0 ; count<BigNumNodes ; count++ )	*(usedn_tmp++) = 0;
	usedn_tmp = usedn;

	TRACE(_T("BM2\n"));

	/* set up little shot table (just defines the connections) */ 
	LittleShots	= (CSurveyShot **)
		malloc( BigConnections	* sizeof(void*));
	little_shot_info = (CSurveyShot *) 
		malloc( BigConnections	* sizeof(CSurveyShot));

	mode = 0;	/* start with looking for a from node */ 
	little_count = 0;
	TRACE(_T("BM6\n"));

	for(count=0 ; count<NumBigShots ; count++){

		last_from = 0;
		BigLegNum = count;
		BigLegTmp = BigShots[BigLegNum];
		BigNodeTmp_A = BigLegTmp->m_FromNode;
		BigNodeTmp_B = BigLegTmp->m_ToNode;

		if(mode == 0){
			if(	(BigNodeTmp_A->m_Position.m_iTotalOfAllShots != 2)
			  || (BigNodeTmp_A->m_Position.m_iNodeType == 1) ){
				(little_shot_info+little_count)->m_FromNode = BigNodeTmp_A;
				LittleShots[little_count] = little_shot_info + little_count;
				mode = 1;
				last_from = 1;
			}
			else{
				if(	(BigNodeTmp_B->m_Position.m_iTotalOfAllShots != 2)
				  || (BigNodeTmp_B->m_Position.m_iNodeType == 1) ){
					(little_shot_info+little_count)->m_FromNode = BigNodeTmp_B;
					LittleShots[little_count] = little_shot_info + little_count;
					mode = 1;
					last_from = 2;
				}
			}
		}

		/* a single shot from one junction to another or to itself
		   we must make sure we don't
		   use the same pointer twice (last_from test)	*/ 
		if(mode == 1){
			if(	(	(BigNodeTmp_A->m_Position.m_iTotalOfAllShots != 2)
			  || (BigNodeTmp_A->m_Position.m_iNodeType == 1) )
			  && last_from != 1){
				(little_shot_info+little_count)->m_ToNode = BigNodeTmp_A;
				mode = 0;
				little_count++;
			}
			else{
				if(	(	(BigNodeTmp_B->m_Position.m_iTotalOfAllShots != 2)
				  || (BigNodeTmp_B->m_Position.m_iNodeType == 1) )
				  && last_from != 2){
					(little_shot_info+little_count)->m_ToNode = BigNodeTmp_B;
					mode = 0;
					little_count++;
				}
			}
		}
	}
	TRACE(_T("BM7\n"));

	/* set up little problem */ 
	/* allocates array, sets LittleNumNodes, fills shot_fr(to)_index,
	   fills array with pointers */ 
	LittleNodes = build_node_table( LittleShots, little_count
	                              , &LittleNodeNum_A, &LittleNodeNum_B
	                              , &LittleNumNodes);

	/* allocates little solver arrays, initializes row & column indices */ 
	init_Lineq( &svv_little
	          , LittleNodes
	          , LittleNodeNum_A
	          , LittleNodeNum_B
	          , little_count
		        , LittleNumNodes);

	/* process all shots */ 
	usedn_tmp = usedn;
	for(count=0 ; count<BigNumNodes ; count++ ) *(usedn_tmp++) = 0;
	usedn_tmp = usedn;

	TRACE(_T("BM7\n"));

	mode = 0; /* means we are starting a new little_leg */ 
	LittleLegNum = 0;

	for(count=0 ; count<NumBigShots ; count++){

		BigLegNum	= count;
		BigLegTmp	= BigShots[BigLegNum];
		BigNodeNumTmp_A = BigNodeNum_A[BigLegNum];
		BigNodeNumTmp_B = BigNodeNum_B[BigLegNum];
		BigNodeTmp_A	= BigLegTmp->m_FromNode;
		BigNodeTmp_B	= BigLegTmp->m_ToNode;

		/* initialize and check which bignode corresponds
		to one of the little nodes */ 
		if(mode == 0){

			LittleLegTmp	     = LittleShots[LittleLegNum];
			LittleNodeNumTmp_A = LittleNodeNum_A [LittleLegNum];
			LittleNodeNumTmp_B = LittleNodeNum_B [LittleLegNum];
			LittleNodeTmp_A	   = LittleLegTmp->m_FromNode;
			LittleNodeTmp_B	   = LittleLegTmp->m_ToNode;
			ConFlag = 0;

			LittleNodeTmp_T    =  (LittleNodeTmp_A == BigNodeTmp_A)
			                   || (LittleNodeTmp_A == BigNodeTmp_B)
			                   ?  LittleNodeTmp_B
			                   :  LittleNodeTmp_A;

			BigNodeTmp_T       =  (LittleNodeTmp_A == BigNodeTmp_A)
			                   || (LittleNodeTmp_B == BigNodeTmp_A)
			                   ?  BigNodeTmp_B
			                   :  BigNodeTmp_A;

			ConFlag =  (LittleNodeTmp_A == BigNodeTmp_A)
			        || (LittleNodeTmp_B == BigNodeTmp_A)
			        ?  1
			        :  (  (LittleNodeTmp_A == BigNodeTmp_B)
			           || (LittleNodeTmp_B == BigNodeTmp_B)
			           ?  2
			           :  0 );
			/* says that we must connect to second big shot */ 

			if(ConFlag == 0){
				ASSERT(FALSE);
				//	printf(_T("\nWhoops! bad junction configuration Hari Kari!"));
				//	getch(); /* TACO doesn't need this */ 
				//	exit(1);
			}

			/* Initialize string submatrix information */ 
			LittleCmat [0]	= 0.0f;
			LittleCmat [1]	= 0.0f;
			LittleCmat [2]	= 0.0f;
			LittleCmat [3]	= 0.0f;
			LittleCmat [4]	= 0.0f;
			LittleCmat [5]	= 0.0f;
			LittleCmat [6]	= 0.0f;
			LittleCmat [7]	= 0.0f;
			LittleCmat [8]	= 0.0f;
			LittleRHS_Tmp[0] = 0.0f;
			LittleRHS_Tmp[1] = 0.0f;
			LittleRHS_Tmp[2] = 0.0f;
			mode = 1; /* means we are now looking for the end of little leg */ 
		}
		else{

			BigNodeTmp_T =  (BigNodeTmp_T == BigNodeTmp_A)
			             || (BigNodeTmp_T == BigNodeTmp_A)
			             ?  BigNodeTmp_B
			             :  BigNodeTmp_A;
		}

		/* setup stuff */ 
		len = BigLegTmp->GetCalculationLength();
		thd = (90.0f - BigLegTmp->GetCalculationCompass()) * deg_rad;
		thd = thd >	pi
		    ? thd - 2.0f * pi
		    : (thd < -pi ? (thd + 2.0f * pi) : thd);
		inc = BigLegTmp->GetCalculationClino() * deg_rad;
		a	= (float) cos(thd); c = (float) sin(thd);
		b	= (float) cos(inc); d = (float) sin(inc);
		asq = a * a;
		bsq = b * b;
		csq = c * c;
		dsq = d * d;

		/* direct calculation of stiffness matrix */ 
		/* cosine matrix = [C] as follows:
		         | cos(azi) cos(inc) ,  sin(azi) , -cos(azi) sin(inc) |
		   [C] = | sin(axi) cos(inc) , -cos(azi) , -sin(azi) sin(inc) |
		         |          sin(inc) ,     0     ,           cos(inc) |

		   stiffness submatrix [K] as follows:
		   [K] = C W C**-1
    */ 

		//TVI
			w1 = 1.0f
			   / BigLegTmp->GetLeg()->GetStandardTapeError();
			w2 = 1.0f
				 / (BigLegTmp->GetLeg()->GetStandardCompassError() * deg_rad * (len < 0.1f ? 0.1f : len));
			w3 = 1.0f
			   / (BigLegTmp->GetLeg()->GetStandardClinoError() * deg_rad * (len < 0.1f ? 0.1f : len));

//		w1 = 1.0f / (len + .05f);
//		w2 = 1.0f / (sdazi * (len < 0.1f ? 0.1f : len));
//		w3 = 1.0f / (sdinc * (len < 0.1f ? 0.1f : len));

		kmat[0] = w1 * asq * bsq + w2 * csq + w3 * asq * dsq;
		kmat[1] = a * c * ( w1 * bsq - w2 + w3 * dsq );
		kmat[2] =	(w1 - w3) * a * b * d;

		kmat[3] =	kmat[1];
		kmat[4] = w1 * bsq * csq + w2 * asq + w3 * csq * dsq;
		kmat[5] = (w1 - w3) * b * c * d;

		kmat[6] =	kmat[2];
		kmat[7] =	kmat[5];
		kmat[8] = w1 * dsq + w3 * bsq;


/*			kmat[0] = w1;
			kmat[1] = 0.0;
			kmat[2] = 0.0;

			kmat[3] = 0;
			kmat[4] = w1;
			kmat[5] = 0;

			kmat[6] = 0;
			kmat[7] = 0;
			kmat[8] = w1;
*/ 

		dp[1] = len * c * b;              /* dy to-from */ 
		dp[0] = len * a * b;              /* dx to-from */ 
		dp[2] = len * d;                  /* dz to-from */ 


		/* mat_disp(&(kmat[0][0]), _T("kmat"),3);*/ 

		/* add submatrix into packed solver matrix */ 
		pmat_insert( &svv_big, &(kmat[0])
		           , BigNodeNumTmp_A
		           , BigNodeNumTmp_A, 1.0f);
		pmat_insert( &svv_big, &(kmat[0])
		           , BigNodeNumTmp_B
		           , BigNodeNumTmp_B, 1.0f);
		pmat_insert( &svv_big, &(kmat[0])
		           , BigNodeNumTmp_A
		           , BigNodeNumTmp_B,-1.0f);
		pmat_insert( &svv_big, &(kmat[0])
		           , BigNodeNumTmp_B
		           , BigNodeNumTmp_A,-1.0f);

		/* set up direct estimate of unknown node position
		   (one is always known with the ordering used) if not constrained
		   (this does little problem too!!) */ 
		if(  (usedn[BigNodeNumTmp_B] == 0)
		  && (BigNodeTmp_B->m_Position.m_iNodeType != 1) ){

			BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[0]
				= BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[0] + dp[0];
			BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[1]
				= BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[1] + dp[1];
			BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[2]
				= BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[2] + dp[2];

			//Added by taco
			BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[3]=1.0f;
		}
		else
		{
			if((usedn[BigNodeNumTmp_A] == 0) && (BigNodeTmp_A->m_Position.m_iNodeType != 1)){

				BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[0]
					= BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[0] - dp[0];
				BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[1]
					= BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[1] - dp[1];
				BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[2]
					= BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[2] - dp[2];

				//By taco
				BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[3]=1.0f;
			}
		}
		usedn[BigNodeNumTmp_A] += 1;
		usedn[BigNodeNumTmp_B] += 1;

		/* invert stiffness matrix and add to little string submatrix */ 
		mat_inv(lmat,kmat,3);
		for(ix=0 ; ix<9 ; ix++) LittleCmat[ix] += lmat[ix];

			/*mat_disp(LittleCmat, _T("LittleCmat build"),3);*/ 

			/* add deltas to little string rhs */ 
			if(BigNodeTmp_T == BigNodeTmp_B){
				LittleRHS_Tmp[0] += dp[0];
				LittleRHS_Tmp[1] += dp[1];
				LittleRHS_Tmp[2] += dp[2];
			}
			else
			{
				LittleRHS_Tmp[0] -= dp[0];
				LittleRHS_Tmp[1] -= dp[1];
				LittleRHS_Tmp[2] -= dp[2];	
			}

			/* set up right hand sides */ 
			mat_x_vect(vect,&(kmat[0]),dp,3);
			vect_add( &(svv_rhs[BigNodeNumTmp_A*3])
			        , &(svv_rhs[BigNodeNumTmp_A*3]),vect,3,2);
			vect_add( &(svv_rhs[BigNodeNumTmp_B*3])
			        , &(svv_rhs[BigNodeNumTmp_B*3]),vect,3,0);

			/* decide if we've arrived at the next junction */ 
			/*printf(_T("\nconflg=%lu "),ConFlag);
			  printf(_T("\nlita=%lu litb=%lu"),LittleNodeTmp_A,LittleNodeTmp_B);
			  printf(_T("\nbiga=%lu bigb=%lu"),BigNodeTmp_A,BigNodeTmp_B);
			  getch();*/ 

			if(  ((ConFlag == 1) && (LittleNodeTmp_T == BigNodeTmp_B))
			  || ((ConFlag == 2) && (LittleNodeTmp_T == BigNodeTmp_A))
			  || (	(ConFlag == 0)
			     && (  (LittleNodeTmp_T == BigNodeTmp_B)
			        || (LittleNodeTmp_T == BigNodeTmp_A) ) ) )
			{

			/* invert little submatrix and insert it */ 
			mat_inv(LittleCmat,LittleCmat,3);
			/*mat_disp(LittleCmat, _T("LittleCmat"),3);*/ 

			/* add little submatrix into packed solver matrix */ 
			pmat_insert( &svv_little
			           , LittleCmat
			           , LittleNodeNumTmp_A
			           , LittleNodeNumTmp_A, 1.0f);
			pmat_insert( &svv_little
			           , LittleCmat
			           , LittleNodeNumTmp_B
			           , LittleNodeNumTmp_B, 1.0f);
			pmat_insert( &svv_little
			           , LittleCmat
			           , LittleNodeNumTmp_A
			           , LittleNodeNumTmp_B,-1.0f);
			pmat_insert( &svv_little
			           , LittleCmat
			           , LittleNodeNumTmp_B
			           , LittleNodeNumTmp_A,-1.0f);

			/* set up right hand sides */ 
			mat_x_vect(LittleRHS_Tmp, LittleCmat, LittleRHS_Tmp, 3);
			vect_add( &(svv_little.m_B[LittleNodeNumTmp_A*3])
			        , &(svv_little.m_B[LittleNodeNumTmp_A*3])
			        , LittleRHS_Tmp, 3, 2);
			vect_add( &(svv_little.m_B[LittleNodeNumTmp_B*3])
			        , &(svv_little.m_B[LittleNodeNumTmp_B*3])
			        , LittleRHS_Tmp, 3, 0);

			/* indicate that we have finished with this string */ 
			LittleLegNum++;
			mode = 0;
		} 
		ConFlag = 0; /* says that we can connect to either big node now */ 

	}

	free(usedn);

	return(TRUE); 
}
/* end build_matrix */ 




//####################################################################
// assess normalised shot error
int CClose::norm_error
	( CSurveyShot **BigShots
	, unsigned long NumBigShots
	, CNode **node_tbl)	
{	
	unsigned long
		BigLegNum, count,
		BigNodeNumTmp_A, BigNodeNumTmp_B;

	CSurveyShot	*BigLegTmp;
	CNode	*BigNodeTmp_A, *BigNodeTmp_B;

	float
		len, thd, inc, dx, dy, dz, sig_len, sig_thd, sig_inc, sig_dev, th,
		a, b, c;

	for(count=0 ; count<NumBigShots ; count++){

		BigLegNum	= count;
		BigLegTmp	= BigShots[BigLegNum];
		BigNodeNumTmp_A = BigNodeNum_A[BigLegNum];
		BigNodeNumTmp_B = BigNodeNum_B[BigLegNum];
		BigNodeTmp_A	= BigLegTmp->m_FromNode;
		BigNodeTmp_B	= BigLegTmp->m_ToNode;

		/* setup stuff */ 
		dx = BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[0]
			 - BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[0];
		dy = BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[1]
			 - BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[1];
		dz = BigNodeTmp_B->m_Position.m_ClosedPosition.m_Items[2]
			 - BigNodeTmp_A->m_Position.m_ClosedPosition.m_Items[2];
		len = (float) sqrt((dx * dx) + (dy * dy) + (dz * dz));
		inc = (float) asin(dz / len);
		b	  = (float) cos(inc);
		a   = dx / len / b;
		c   = dy / len / b;
		thd = 90.0f
		    - (a >= 0.0f ? (float) asin(c) : pi - (float) asin(c)) / deg_rad;
		thd = thd < -0.0001f ? thd + 360.0f : (thd >= 359.9999f ? thd - 360.0f : thd);
		inc = inc / deg_rad;
		th      = BigLegTmp->GetCalculationCompass();
		th      = th >= 359.9999f ? th - 360.0f : (th < 0.0001f ? th + 360.0f : th);

		sig_len = (len - BigLegTmp->GetCalculationLength())
		        / BigLegTmp->GetLeg()->GetStandardTapeError();
		sig_len = sig_len > 0.0f ? sig_len : -sig_len;

		sig_thd = (thd - th);
		sig_thd = sig_thd > 180.0f
		        ? sig_thd - 360.0f
						: ( sig_thd < -180.0f
		          ? sig_thd + 360.0f
							: sig_thd);
		sig_thd *= b;
		sig_thd = sig_thd > 0.0f ? sig_thd : -sig_thd;
		sig_thd = sig_thd	/ BigLegTmp->GetLeg()->GetStandardCompassError();

		sig_inc = (inc - BigLegTmp->GetCalculationClino())
		        / BigLegTmp->GetLeg()->GetStandardClinoError();
		sig_inc = sig_inc > 0.0f ? sig_inc : -sig_inc;

		if(sig_len > sig_thd){
			if(sig_thd > sig_inc){
				sig_dev = sig_len - sig_thd;
			}else{
				sig_dev = sig_len - sig_inc;
			}
		}else{
			if(sig_len > sig_inc){
				sig_dev = sig_thd - sig_len;
			}else{
				sig_dev = sig_thd - sig_inc;
			}
		}
		sig_dev = sig_dev > 0.0f ? sig_dev : -sig_dev;

		BigLegTmp->SetStressCompass(sig_thd);
		BigLegTmp->SetStressClino(sig_inc);
		BigLegTmp->SetStressTape(sig_len);
		BigLegTmp->SetStressDev(sig_dev);      // Measure of max pure error
		if(sig_dev > 2.0f){
			TRACE(_T("\n  leg %li from node =%s  to node =%s  "), count
	     , BigLegTmp->m_FromNode->m_szNodeName
	     , BigLegTmp->m_ToNode->m_szNodeName);
			TRACE(_T("\n count=%li  sig_len=%f  sig_thd=%f  sig_inc=%f  sig_dev=%f")
			     , count, sig_len, sig_thd, sig_inc, sig_dev);
		}
	}


	return(TRUE); 
}
/* end norm_error */ 


//####################################################################
//  apply constraints to fixed nodes
int CClose::constr_equ
	( struct Lin_eq *matrix
	, unsigned long NumNodes
	, CNode	**node_tbl
	, int level)
{
	unsigned long
		node, *col0, *col1, node3, ix, iy, row, ind, ind1;

	CNode	*NodeP;

	float *val0, *val1, *val2;

	/* solver storage pointers */ 
	float **svv_xp, *svv_rhs;
	float *svv_D,	*svv_a;
	unsigned long *svv_c;
	unsigned long *svv_ri;

	/* solver linear equation pointers
	   - don't want to reference through structure all the time */ 
	svv_xp	= matrix->m_ANS;
	svv_rhs = matrix->m_B;
	svv_D		=	matrix->m_D;
	svv_a		=	matrix->m_A;
	svv_c		=	matrix->m_C;
	svv_ri	= matrix->m_RI;

	/* process all nodes */ 
	for(node = 0 ; node < NumNodes ; node++)
	{
		node3 = node*3;
		NodeP = node_tbl[node];

		/* set up pointers to answers */ 
		*(svv_xp+node3)   = &(NodeP->m_Position.m_ClosedPosition.m_Items[0]);
		*(svv_xp+node3+1) = &(NodeP->m_Position.m_ClosedPosition.m_Items[1]);
		*(svv_xp+node3+2) = &(NodeP->m_Position.m_ClosedPosition.m_Items[2]);

		/* process all constrained nodes */ 
		if(  (NodeP->m_Position.m_iNodeType > 0)
		  && (NodeP->m_Position.m_iNodeType <= level) )
		{

			/* set right hand sides to position values */ 
			svv_rhs[node3]   = NodeP->m_Position.m_ClosedPosition.m_Items[0];
			svv_rhs[node3+1] = NodeP->m_Position.m_ClosedPosition.m_Items[1];
			svv_rhs[node3+2] = NodeP->m_Position.m_ClosedPosition.m_Items[2];

			/* modify all right hand sides to restore symmetry */ 
			/* scan through the three columns corresponding with "node" */ 

			/* since matrix is symmetric, can use row values 
			   rather than column values, in future we won't
			   even store the lower triangular matrix
			   "column" is the column index corresponding with "value" */ 

			/* all 3 (rows)cols will have same entries */ 
			// index low and high limits
			col0 = svv_c + svv_ri[node3]; 
			col1 = svv_c + svv_ri[node3+1];

			// point to node's rows in array "a"
			val0 = svv_a + svv_ri[node3];
			val1 = svv_a + svv_ri[node3+1];
			val2 = svv_a + svv_ri[node3+2];

			/* test for col allows us to get only elements for this row */ 
			for( ; col0 < col1 ; col0++)
			{

				/* don't adjust rows belonging to "node" as it must stay put */ 
				if((*col0 < node3) || (*col0 >= (node+1)*3))
				{
					svv_rhs[*col0]	-= *val0 * svv_rhs[node3];
					svv_rhs[*col0]	-= *val1 * svv_rhs[node3+1];
					svv_rhs[*col0]	-= *val2 * svv_rhs[node3+2];
					*val0++ = 0.0f;
					*val1++ = 0.0f;
					*val2++ = 0.0f;
				}
				else
				{
					*val0++ = *col0 == node3	? 1.0f : 0.0f;
					*val1++ = *col0 == node3+1 ? 1.0f : 0.0f;
					*val2++ = *col0 == node3+2 ? 1.0f : 0.0f;
					svv_D[node3]	= 1.0f;
					svv_D[node3+1] = 1.0f;
					svv_D[node3+2] = 1.0f;
				}
			}

			/* eliminate other values in matrix corresponding to node */
			/* iy is now index to target row number */
			for(iy=node3 ; iy<node3+3 ; iy++){

				/* ind is index to now target row number */
				ind1 = svv_ri[iy+1];
				for(ind = svv_ri[iy]; ind < ind1 ; ind++){
					row = svv_c[ind];

					for(ix = svv_ri[row]; ix < svv_ri[row+1]; ix++){

						if(svv_c[ix] == iy){

							/* don't change values in rows belonging to node */
							if(svv_c[ix] != row) svv_a[ix] = 0.0f;
						}
					}
				}
			}
		}
	}
	return TRUE;
}

			/* eliminate other values in matrix corresponding to node 
			for(ix=0 ; ix<NumNodes*3 ; ix++)
			{

				/* don't change values in rows belonging to node 
				if((ix < node3) || (ix > node3+2))
				{
					col0 = svv_c + svv_ri[ix];
					col1 = svv_c + svv_ri[ix+1];
					val0 = svv_a + svv_ri[ix];
					for( ; col0 < col1 ; )
					{
						if((*col0 >= node3) && (*col0 < node3+3)) *val0 = 0.0f;
						col0++; val0++;
					}
				}
			}
*/

//####################################################################
//  conjugate-gradient matrix solver */ 
int CClose::solve
	( struct Lin_eq *matrix
	,	unsigned long	numeq )
{

	float	s1, s2, alpha, tol;
	unsigned long	iy, k, i1, i2, limit, *RItmp, *Ctmp, ia;

	float	*Dtmp, *X, *Xtmp, curr_D, *m_Btmp, *Atmp, *P, *Q, *Ptmp,
	      *Qtmp, dmax,	tmp;

	float	**m_ANS, *m_B,	*m_D,	*m_A;
	unsigned long	*m_C,	*m_RI;

	/* set up stuff - don't want to use structure reference after here */ 
	m_ANS	= matrix->m_ANS;
	m_B		= matrix->m_B;
	m_D		=	matrix->m_D;
	m_A		=	matrix->m_A;
	m_C		=	matrix->m_C;
	m_RI	= matrix->m_RI;

	X				= (float*)	malloc(numeq * sizeof(float));
	Q				= (float*)	malloc(numeq * sizeof(float));
	P				= (float*)	malloc(numeq * sizeof(float));
	Ptmp	= P;
	Xtmp	= X;
	Dtmp	= m_D;
	m_Btmp	= m_B;

	/* normalize diagonal and answer vectors */ 
	for (iy=0 ; iy<numeq ; iy++) {
		*Dtmp = (float)sqrt(*Dtmp);
		*(Xtmp++) = *(m_ANS[iy]) * (*Dtmp);

		*m_Btmp = *m_Btmp / (*Dtmp++);
		m_Btmp++;
	}

	/* each row */ 
	Atmp	= m_A;
	Ctmp	= m_C;
	RItmp = m_RI;
	m_Btmp	= m_B;
	Qtmp	= Q;
	Ptmp	= P;
	s1 = 0.0f;
	for (iy=0 ; iy<numeq ; iy++){

		curr_D = m_D[iy];

		i1 = *RItmp;
		i2 = *((RItmp++)+1);
		if (i1 != i2) {

			/* each A element */ 
			for (ia=i1 ; ia < i2 ; ia++){

				*Atmp /= (curr_D * m_D[*Ctmp]);
				*m_Btmp -= *(Atmp++) * X[*(Ctmp++)];
			}
			*(Ptmp++) = *m_Btmp;
			s1 = s1 + ((*m_Btmp) * (*(m_Btmp)));
			m_Btmp++;
		}
	}

	/* solve the matrix */ 
	tol = 0.001f;
	s2=1.0f;
	limit = numeq+1;

	for (k=0 ; (k<limit) && (s2 > tol); k++){
		alpha = 0.0f;

		RItmp = m_RI;
		Qtmp	= Q;
		Ptmp	= P;
		Atmp	= m_A;
		Ctmp	= m_C;
		for (iy=0 ; iy<numeq ; iy++) {

			i1 = *RItmp;
			i2 = *((RItmp++)+1);
			if (i1 != i2) {

				/* each A element */ 
				*Qtmp = 0.0f;	/* *Ptmp;	zero ??? */ 
				for (ia=i1 ; ia < i2 ; ia++) *Qtmp += *(Atmp++) * P[*(Ctmp++)];
				alpha += *(Qtmp++) * (*(Ptmp++));
			}
		}
		alpha = s1 / (alpha+1e-20f);

		s2 = 0.0f;
		Xtmp = X;
		Ptmp = P;
		m_Btmp = m_B;
		Qtmp = Q;
		Dtmp = m_D;
		dmax = 0.0f;
		for (iy=0 ; iy<numeq ; iy++){
			tmp = alpha * (*(Ptmp++));
			dmax = fabs(tmp / (*Dtmp)) > fabs(dmax)
			     ? tmp / (*Dtmp)
			     : dmax ;
			Dtmp++;
			*(Xtmp++) += tmp;
			*m_Btmp	-= alpha * (*(Qtmp++));
			s2		+= *m_Btmp * (*m_Btmp);
			m_Btmp++;
		}

		alpha = s2 / (s1+1e-20f);
		s1 = s2;
		//Pierre's closure error fix
		if (s2/500000 > tol)
		{
			tol = s2/500000;
		}

		Ptmp = P;
		m_Btmp = m_B;
		for (iy=0 ; iy<numeq ; iy++){
			*Ptmp = *(m_Btmp++) + alpha * (*Ptmp);
			*Ptmp = fabs(*Ptmp)<1e-15f ? 1e-15f : *Ptmp;
			Ptmp++;
		}

		//printf(_T("\nEnd Solver iter %li s2=%16.6f	delta=%16.6f")
		//      , k,s2,dmax);
	}

	/* un-normalize solution */ 
	Xtmp = X;
	Dtmp = m_D;

	//TRACE(_T("\nSolution!!"));
	for (iy=0 ; iy<numeq ; iy++){
		*(m_ANS[iy]) = *(Xtmp++) / *(Dtmp++);
		//TRACE(_T("\n rowwww = %li  value = %f "), iy, *(m_ANS[iy]));
	}
	//TRACE(_T("\nEnd Solution"));

	free(X);
	free(P);
	free(Q);

	if(k == limit)
	{
		TRACE(_T("\nBad News!!"));
		ASSERT(FALSE);
		return FALSE;
	}

	return(TRUE);
}



//####################################################################
//  Sparse Elimination matrix solver */ 
int CClose::solveband
	( struct Lin_eq *matrix
	,	unsigned long	numeq )
{

	unsigned long	
		ColumnIndexLow ,	ColumnIndexHigh , ColumnIndex , ColumnIndexDiag,
		ColumnIndexLow3,	ColumnIndexHigh3, ColumnIndex3,
		ColumnIndex2    ,
		row, RowIndex, 
		TargetRow, TargetColumn;

	float	*m_Btmp,
	      tmp, rowfactor;

	float	**m_ANS, *m_B,	*m_D,	*m_A;
	unsigned long	*m_C,	*m_RI;

	/* set up stuff - don't want to use structure reference after here */ 
	m_ANS  = matrix->m_ANS;
	m_B    = matrix->m_B;
	m_D    = matrix->m_D;
	m_A    = matrix->m_A;
	m_C    = matrix->m_C;
	m_RI   = matrix->m_RI;

	m_Btmp = m_B;

	// Ax = B
	// forward pass in the matrix - eliminate lower off-diagonal elements
	// This is specifically geared to solve a bi-dependent problem
	// by this I mean that since all junction nodes are solved only two
	// nodes contribute to any element in the current matrix - therefore
	// during the solution no new off-diagonal elements will be introduced
	// In this way, we are free of storage issues

/*
	for (row=0 ; row<numeq ; row++) {
		TRACE(_T("\n row %8li  "),row);
		ColumnIndexLow = m_RI[row];
		ColumnIndexHigh  = m_RI[row+1] - 1;

		for (RowIndex = 0 ; RowIndex < numeq ; RowIndex++){

			for (ColumnIndex = ColumnIndexLow ;
			     ColumnIndex <= ColumnIndexHigh ;
			     ColumnIndex++){
				if(m_C[ColumnIndex] == RowIndex) break;
			}
			if(ColumnIndex <= ColumnIndexHigh){
				TRACE(_T(" %10.4f"), m_A[ColumnIndex]);
			}else{
				TRACE(_T(" %10.4f"), 0.0f);
			}
		}
		TRACE(_T("  RHS value= %10.4f "),m_B[row]);
	}
	
	TRACE(_T("\n  end loop A\n"));
*/


	for (row=0 ; row<numeq ; row++) {

		// Get column indices - low, high & diag
    // In future, we'll only have the upper tri-diagonal
		ColumnIndexLow  = m_RI[row];
		ColumnIndexHigh = m_RI[row+1] - 1;

		for (ColumnIndex =  ColumnIndexLow ;
		     ColumnIndex <= ColumnIndexHigh ;
		     ColumnIndex++){
			if(m_C[ColumnIndex] == row) break;
		}
		if(ColumnIndex <= ColumnIndexHigh){
			ColumnIndexDiag = ColumnIndex;
		}else{
			TRACE(_T("\nElimination Solver: Bad Index "));
			ASSERT(FALSE);
			return FALSE;
		}


		// Step through each row
		for (RowIndex = ColumnIndexLow ;
		     RowIndex <= ColumnIndexHigh ;
		     RowIndex++){
			TargetRow = m_C[RowIndex];

			if(TargetRow > numeq){
				TRACE(_T("\nElimination Solver: Bad Target Row"));
				ASSERT(FALSE);
				return FALSE;
			}

			// Process only if target row is greater than current row
			if(TargetRow > row){

				// Get a factor to use to multiply source row by before subtracting it
				// m_A[RowIndex] is same as m_A of the TargetRow below the diag element!
				// (symmetry!)
				rowfactor = m_A[RowIndex] / m_A[ColumnIndexDiag];

				// Go through all columns (cause the column number is kinda random!
				// the only consistent thing is the diagonal triple matrix is first!
				for(ColumnIndex2 = ColumnIndexLow ;
				    ColumnIndex2 <= ColumnIndexHigh ;
						ColumnIndex2++){
					TargetColumn = m_C[ColumnIndex2];

					if(TargetColumn > numeq){
						TRACE(_T("\nElimination Solver: Bad Target Column"));
						ASSERT(FALSE);
						return FALSE;
					}

					// Only process elements above the diagonal
					if(TargetColumn >= TargetRow){

						// Initialize target row scan index
						ColumnIndexLow3  = m_RI[TargetRow];
						ColumnIndexHigh3 = m_RI[TargetRow+1]-1;

						// Need to scan target row until target column is found
						for (ColumnIndex3 = ColumnIndexLow3 ;
							   ColumnIndex3 <= ColumnIndexHigh3 ;
								 ColumnIndex3++){
							if(m_C[ColumnIndex3] == TargetColumn) break;
						}

						// Fix column value by subtracting scaled "row" values
						m_A[ColumnIndex3] = m_A[ColumnIndex3] 
						                  - m_A[ColumnIndex2] * rowfactor;
// TRACE(_T("     row= %li Tcol= %li  Value = %f \n"),TargetRow,TargetColumn,m_A[ColumnIndex3]);
					}
				}
				// Fix the right hand side
				m_B[TargetRow] -= m_B[row] * rowfactor;
// TRACE(_T("     row= %li Tcol= RHS  Value = %f \n"),TargetRow,m_B[TargetRow]);
			}
		}

		// Normalise the current row
		tmp = 1.0f / m_A[ColumnIndexDiag];
		for ( ColumnIndex = ColumnIndexLow;
		      ColumnIndex<= ColumnIndexHigh    ;
				  ColumnIndex++ ) {
			m_A[ColumnIndex] *= tmp;
		}
		// fix RHS
// TRACE(_T("           row= %li Tcol= RHS  Value = %f  tmp= %f \n"),row,m_B[row],tmp);
		m_B[row] *= tmp;
// TRACE(_T("Normalised row= %li Tcol= RHS  Value = %f \n"),row,m_B[row]);
	}

/*
	for (row=0 ; row<numeq ; row++) {
		TRACE(_T("\n row %8li  "),row);
		ColumnIndexLow = m_RI[row];
		ColumnIndexHigh  = m_RI[row+1] - 1;

		for (RowIndex = 0 ; RowIndex < numeq ; RowIndex++){

			for (ColumnIndex = ColumnIndexLow ;
			     ColumnIndex <= ColumnIndexHigh ;
			     ColumnIndex++){
				if(m_C[ColumnIndex] == RowIndex) break;
			}
			if(ColumnIndex <= ColumnIndexHigh){
				TRACE(_T(" %10.4f"), m_A[ColumnIndex]);
			}else{
				TRACE(_T(" %10.4f"), 0.0f);
			}
		}
		TRACE(_T("  RHS value= %10.4f "),m_B[row]);
	}
	TRACE(_T("\n  end loop B\n"));*/

// ********************************************************************

 	for (row=numeq-1 ; (row<numeq) && (row>=0) ; row--) {  // when row goes below zero, stop!!
// TRACE(_T("band 2nd pass row= %li \n"),row);
//TRACE(_T("     loop:     row= %li numeq = %li \n"),row,numeq);

		// Get column indices - low, high & diag
    // In future, we'll only have the upper tri-diagonal
		ColumnIndexLow  = m_RI[row];
		ColumnIndexHigh = m_RI[row+1] - 1;

		for (ColumnIndex = ColumnIndexLow ;
		     ColumnIndex <= ColumnIndexHigh ;
				 ColumnIndex++){
			if(m_C[ColumnIndex] == row) break;
		}
		ColumnIndexDiag = ColumnIndex;


		// Step through each Target row
		for (RowIndex = ColumnIndexLow ;
		     RowIndex <= ColumnIndexHigh ;
		     RowIndex++){
			TargetRow = m_C[RowIndex];

			// Process only if target row is less than current row
			if(TargetRow < row){

				// Initialize target row scan index
				ColumnIndexLow3  = m_RI[TargetRow];
				ColumnIndexHigh3 = m_RI[TargetRow+1]-1;

				// TargetColumn = row;  // only have one target row for this pass!

				// Need to scan target row until target column is found
				for (ColumnIndex3 = ColumnIndexLow3 ;
					   ColumnIndex3 <= ColumnIndexHigh3 ;
						 ColumnIndex3++){
					if(m_C[ColumnIndex3] == row) break; // TargetColumn is row
				}
				if(ColumnIndex3 > ColumnIndexHigh3){
					TRACE(_T("SolveBand: Severe Error: No target column found! row = %li col = %li \n")
						,TargetRow,TargetColumn);
					ASSERT(FALSE);
					return FALSE;
				}


// TRACE(_T("     Trow= %li Tcol= %li  Value = %f \n"),TargetRow,TargetColumn,m_A[ColumnIndex3]);
// TRACE(_T("         m_B[row] Value = %f Targ=  %f \n"),m_B[row],m_B[TargetRow]);

				// Fix the right hand side
				m_B[TargetRow] -= m_B[row] * m_A[ColumnIndex3];

//				TRACE(_T("         m_B[row] Value = %f Targ=  %f \n"),m_B[row],m_B[TargetRow]);
			}
		}
//TRACE(_T("final     row= %li Tcol= RHS  Value = %f \n"),row,m_B[row]);
		*(m_ANS[row]) = m_B[row];

/*		testit = AfxCheckMemory();
		if(!testit){
					TRACE(_T("\nSolveBand: Memory Error \n"));
					ASSERT(FALSE);
					return FALSE;
		}*/
	}

 TRACE(_T("done loop:     row= %li numeq = %li \n"),row,numeq);

/*
	for (row=0 ; row<numeq ; row++) {
		TRACE(_T("\n row %8li  "),row);
		ColumnIndexLow = m_RI[row];
		ColumnIndexHigh  = m_RI[row+1] - 1;

		for (RowIndex = 0 ; RowIndex < numeq ; RowIndex++){

			for (ColumnIndex = ColumnIndexLow ;
			     ColumnIndex <= ColumnIndexHigh ;
			     ColumnIndex++){
				if(m_C[ColumnIndex] == RowIndex) break;
			}
			if(ColumnIndex <= ColumnIndexHigh){
				TRACE(_T(" %10.4f"), m_A[ColumnIndex]);
			}else{
				TRACE(_T(" %10.4f"), 0.0f);
			}
		}
		TRACE(_T("  RHS value= %10.4f "),m_B[row]);
	}
	TRACE(_T("\n  end loop C\n"));
*/

/*
	if(1 > 2)
	{
		TRACE(_T("\nBad News!!"));
		ASSERT(FALSE);
		return FALSE;
	}
*/ 
	return(TRUE);
}



//####################################################################
// Allocate and initialise solver arrays
int CClose::init_Lineq
	( struct Lin_eq	*matrix
	, CNode	**nodes
	, unsigned long *shot_fr
	, unsigned long *shot_to
	, unsigned long numshots
	, unsigned long numnodes)
{

	TRACE(_T("InitLineq %li shots %li nodes\n"),numshots,numnodes);

	unsigned long
		ix, nx, na, *ri, nc, node1, node2, r1, r2, r3, elem, ia, numeq;

	nx = (numnodes + 1) * 3;
	na = 9 * (numnodes + 1) + 9 * (numshots + 1) * 2;
	numeq = numnodes * 3;

	matrix->m_ANS	= (float**)        malloc(nx     * sizeof(float*));
	matrix->m_B		= (float*)         malloc(nx     * sizeof(float));
	matrix->m_RI	= (unsigned long*) malloc((nx+1) * sizeof(long));
	matrix->m_D	  = (float*)         malloc(nx     * sizeof(float));

	matrix->m_A   = (float*)         malloc(na     * sizeof(float));
	matrix->m_C   = (unsigned long*) malloc(na     * sizeof(long));

	//	ASSERT(AfxCheckMemory());	
	if(matrix->m_C == 0L){
		TRACE(_T("Out of memory in solver"));
		return(FALSE);
	}

	for(ix=0 ; ix < nx ; ix++){
		*(matrix->m_ANS+ix) = 0;
		*(matrix->m_B+ix) = 0.0f;
		*(matrix->m_D+ix) = 0.0f;
	}

	//		ASSERT(AfxCheckMemory());	
	for(ix=0 ; ix < na ; ix++){
		matrix->m_A[ix] = 0.0f;
		matrix->m_C[ix] = (unsigned long)-1;	
		/* ??? this might be a glitch with some geoms */  
	}

	/* Initialize matrix row indices and diagonal column indices */  
	elem = 0;
	ri = matrix->m_RI;
	for(ix=0 ; ix<numnodes ; ix++){

		/* TotalOfAllShots is the number of connections into node 
		   we need one 3x3 element of space allocated for each 
		   (plus diagonal element) */ 
		nc = 3 * (1 + (*(nodes++))->m_Position.m_iTotalOfAllShots);

		matrix->m_C[elem]			= 3*ix;
		matrix->m_C[elem+1]	= 3*ix+1;
		matrix->m_C[elem+2]	= 3*ix+2;
		*ri++ = elem; elem += nc;
		matrix->m_C[elem]			= 3*ix;
		matrix->m_C[elem+1]	= 3*ix+1;
		matrix->m_C[elem+2]	= 3*ix+2;
		*ri++ = elem; elem += nc;
		matrix->m_C[elem]			= 3*ix;
		matrix->m_C[elem+1]	= 3*ix+1;
		matrix->m_C[elem+2]	= 3*ix+2;
		*ri++ = elem; elem += nc;
	}
	//		ASSERT(AfxCheckMemory());

	*ri = elem; /* one extra value marks end */  

	/* Initialize column indices	for off-diagonal elements */  
	for(ix=0 ; ix<numshots ; ix++)
	{
		node1 = 3 * shot_fr[ix];
		node2 = 3 * shot_to[ix];

		/* get first empty location in node 1 rows */  
		r1 = matrix->m_RI[node1];
		r2 = matrix->m_RI[node1+1];
		r3 = matrix->m_RI[node1+2];
		for (ia=r2 ; (r1<ia) && (matrix->m_C[r1] != -1) ; r1++)
		{
			r2++;
			r3++;
		}

		matrix->m_C[r1]   = node2;
		matrix->m_C[r1+1] = node2+1;
		matrix->m_C[r1+2] = node2+2;

		matrix->m_C[r2]   = node2;
		matrix->m_C[r2+1] = node2+1;
		matrix->m_C[r2+2] = node2+2;

		matrix->m_C[r3]   = node2;
		matrix->m_C[r3+1] = node2+1;
		matrix->m_C[r3+2] = node2+2;

		/* get first empty location in node 2 rows */ 
		r1 = matrix->m_RI[node2];
		r2 = matrix->m_RI[node2+1];
		r3 = matrix->m_RI[node2+2];

			
		for(ia=r2 ; (r1<ia) && (matrix->m_C[r1] != -1) ; r1++)
		{r2++;r3++;};

		matrix->m_C[r1]   = node1;
		matrix->m_C[r1+1] = node1+1;
		matrix->m_C[r1+2] = node1+2;

		matrix->m_C[r2]   = node1;
		matrix->m_C[r2+1] = node1+1;
		matrix->m_C[r2+2] = node1+2;

		matrix->m_C[r3]   = node1;
		matrix->m_C[r3+1] = node1+1;
		matrix->m_C[r3+2] = node1+2;
	}

	return(TRUE);
}



//####################################################################
// Free solver arrays
void CClose::free_Lineq
	(struct Lin_eq *matrix)
{
	free(matrix->m_ANS);
	free(matrix->m_B);
	free(matrix->m_RI);
	free(matrix->m_D);
	free(matrix->m_A);
	free(matrix->m_C);
}


//####################################################################
// Mark nodes whose position is solved
void	CClose::mark_solved
	( CNode **NodeList
	, unsigned long NumNodes
	, int mark)
{
	unsigned long ix;
	for(ix=0 ; ix<NumNodes ; ix++)
		if((*(NodeList+ix))->m_Position.m_iNodeType == 0)
		(*(NodeList+ix))->m_Position.m_iNodeType = mark;
}


//####################################################################
// allocates array, sets BigNumNodes, fills shot_fr(to)_index,
// fills array with pointers
CNode ** CClose::build_node_table
	( CSurveyShot **shots
	, unsigned long numshots
	, unsigned long **shot_fr_temp
	, unsigned long **shot_to_temp
	, unsigned long *solv_numnodes)
{
	TRACE(_T("build_node_table %li shots\n"),numshots);	

	unsigned long	ix, numnodes;
	long	iy={0};
	CNode	**nodes, **node_tmp;
	CSurveyShot *shot;

	nodes = (CNode	**) malloc((1 + numshots) * sizeof(void*));
	*shot_fr_temp = (unsigned long*) malloc(numshots * sizeof(long*));
	*shot_to_temp = (unsigned long*) malloc(numshots * sizeof(long*));

	node_tmp = nodes;
	if(shots[0]->m_FromNode->m_Position.m_iNodeType == 1){
		*(node_tmp++) = shots[0]->m_FromNode;
		*(node_tmp)   = shots[0]->m_ToNode;
		(*shot_fr_temp)[0] = 0;
		(*shot_to_temp)[0] = 1;
	}else{
		*(node_tmp++) = shots[0]->m_ToNode;
		*(node_tmp)   = shots[0]->m_FromNode;
		(*shot_fr_temp)[0] = 1;
		(*shot_to_temp)[0] = 0;
	}
	numnodes = 2;
/*	TRACE(_T("\n  leg 0 from node =%s  to node =%s  ")
	     , shots[0]->m_FromNode->m_szNodeName
	     , shots[0]->m_ToNode->m_szNodeName);*/

	for(ix=1 ; ix<numshots ; ix++){
		shot = shots[ix];
		node_tmp = nodes + numnodes - 1;
/*	TRACE(_T("\n  leg %li from node =%s  to node =%s  "), ix
	     , shots[ix]->m_FromNode->m_szNodeName
	     , shots[ix]->m_ToNode->m_szNodeName);*/

		for(iy=numnodes-1 ; iy >= 0 ; iy--){
			if(shot->m_FromNode == *(node_tmp--)) break;
		}

		// Is new node
		if(iy < 0){
			nodes[numnodes] = shot->m_FromNode;
			iy = numnodes;
			numnodes++;
		}
		(*shot_fr_temp)[ix] = iy;

		node_tmp = nodes + numnodes - 1;
		for(iy=numnodes-1 ; iy >= 0 ; iy--){
			if(shot->m_ToNode == *(node_tmp--)) break;
		}
		if(iy < 0){
			nodes[numnodes] = shot->m_ToNode;
			iy = numnodes;
			numnodes++;
		}
		(*shot_to_temp)[ix] = iy;
	}
	*solv_numnodes = numnodes;

	return(nodes);
}


//####################################################################
// Supervises (steps through) loop closure
int	CClose::surv_loop_close
	( CSurveyShot **ori_shots
	, unsigned long numshots
	,int iStage)
{
	unsigned long iy;
	CSurveyShot **big_shots;

	ASSERT(AfxCheckMemory());
	switch(iStage)
	{
	case 0:
		/* allocates array, sets BigNumNodes, fills shot_fr(to)_index,
		fills array with pointers */ 
		BigNodes = build_node_table( ori_shots, numshots
		                           , &BigNodeNum_A, &BigNodeNum_B, &BigNumNodes);
		ASSERT(AfxCheckMemory());

		/* get a good order for shots and nodes */ 
		BigOrder = good_order(ori_shots, numshots, BigNodes);
		free(BigNodes);
		free(BigNodeNum_A);
		free(BigNodeNum_B);

		big_shots = (CSurveyShot **) malloc(numshots * sizeof(void *));
		for(iy=0 ; iy < numshots; iy++){
			big_shots[iy] = ori_shots[BigOrder[iy]];
		}
		free(BigOrder);

		/* allocates array, sets BigNumNodes, fills shot_fr(to)_index,
		fills array with pointers */ 
		BigNodes = build_node_table( big_shots, numshots
		                           , &BigNodeNum_A, &BigNodeNum_B, &BigNumNodes);
		ASSERT(AfxCheckMemory());

		/* allocates big solver arrays, initializes row & column indices */ 
		if ( !init_Lineq( &svv_big, BigNodes, BigNodeNum_A, BigNodeNum_B
		   , numshots, BigNumNodes))
		{
					return FALSE;
		}
		ASSERT(AfxCheckMemory());
//		break;

//	case 1:

		/* fills big and little solver arrays */ 
		if (!build_matrix(big_shots, numshots, BigNodes))
			return FALSE;

		if(LittleNumNodes > 2){
			/* solve little array problem */ 
			if (!constr_equ( &svv_little, LittleNumNodes, LittleNodes, 2)) return FALSE;
			if (!solve(&svv_little,	LittleNumNodes * 3)) return FALSE;
			mark_solved(LittleNodes, LittleNumNodes, 2);
		}
//		break;

//	case 2:
		/* apply constraints from little problem to big problem */ 
		if (!constr_equ( &svv_big, BigNumNodes, BigNodes, 3))
			return FALSE;
//		break;

//	case 3:
		/* solve big array problem */ 
		if (!solveband(&svv_big, BigNumNodes * 3))
			return FALSE;
		mark_solved(BigNodes, BigNumNodes, 3);

		// determine normalised error
		if (!norm_error(big_shots, numshots, BigNodes))
			return FALSE;
//		break;

//	case 4:
		/* free big and little arrays */ 
		ASSERT(AfxCheckMemory());
		free_Lineq(&svv_big);
		free_Lineq(&svv_little);
		free(BigNodes);
		free(BigNodeNum_A);
		free(BigNodeNum_B);
		free(LittleNodes);
		free(big_shots);
//		break;
	}
	return(TRUE);
}

CClose::CClose(int iColorScheme)
{
	m_iColorScheme=iColorScheme;
}

