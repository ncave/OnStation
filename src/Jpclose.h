// (C) Copyright 1996, Pierre Lebel
#ifndef I_JPCLOSE__
#define I_JPCLOSE__

//#define MAX_STATION_NAME_LENGTH 6
//a#define MAX_COLOR_SCHEMES 1

class CNode;
class CSurveyShot;

/* solver linear equation structure */
struct  Lin_eq
{                         // Ax = B
	float         **m_ANS;  // Solution Vector Pointer Array (x)
	float		      *m_B;     // B
	float         *m_D;     // D is Diagonal Vector length n
	float         *m_A;     // A is packed square (nxn) symetric matrix
	unsigned long *m_C;     // Specifies the column index of each element in A
	unsigned long *m_RI;    // Vector length n 
	                        // specifyies the start location of each row
};
#endif

class CClose
{
private:
	int m_iColorScheme;
public:
	CClose(int iColorScheme);

	int mat_inv(float *to,
            float *from,
            unsigned int size);  /* invert square matrix from & to can be same */

	int mat_x_mat(float *to,
              float *from1,
              float *from2,
              unsigned int size);        /* cross multiply two (square) matrices */
                                                                                                                        /* all three can be same matrix */
	int mat_x_vect(float *to,
               float *mat,
               float *vect,
               unsigned int size);       /* cross multiply square matrix and vector*/
	int vect_add(float *to,
             float *vect1,
             float *vect2,
             unsigned int size,
             unsigned int oper); /* add two vectors */
                                                                                                                        /* to can be same as either vect */
	int mat_disp(float *from,
             TCHAR *name,
             unsigned int size); /* display (square) matrix */
                                                                                                                        /* oper : 0 add both 1 sub first,
                                                                                                                                        2 sub 2nd  3 sub both  */
	int pmat_insert(struct Lin_eq *matrix,
                                                                                                        float *sub,
                          unsigned long row,
                          unsigned long col,
                          float fact);   /* scale and add submatrix */


	int pmat_dump(struct Lin_eq *matrix, unsigned long num); /* dump packed matrix to file */


	int vect_dump(float *from,
              unsigned long size);       /* dump packed matrix to file */


	int build_matrix( CSurveyShot **BigShots
                , unsigned long NumBigShots
                , CNode  **node_tbl);      /* build matrix to solve for station coordinates */

	int norm_error( CSurveyShot **BigShots
                , unsigned long NumBigShots
                , CNode  **node_tbl);      /* estimate normalised instrument error */

	long* good_order( CSurveyShot **BigShots
                , unsigned long NumBigShots
                , CNode  **node_tbl);      /* build matrix to solve for station coordinates */

	int constr_equ( struct Lin_eq *matrix
             , unsigned long NumNodes
             , CNode  **node_tbl
             , int level);       /* build matrix to solve for station coordinates */


	int solve       ( struct Lin_eq *matrix
                  ,       unsigned long   numeq ); /* conjugate-gradient matrix solver */

	int solveband   ( struct Lin_eq *matrix
                  ,       unsigned long   numeq ); /* elimination matrix solver */

	int init_Lineq( struct Lin_eq  *matrix
              , CNode  **nodes
              , unsigned long *shot_fr
              , unsigned long *shot_to
              , unsigned long numshots
              , unsigned long numnodes);

	void free_Lineq(struct Lin_eq *matrix);


	CNode  **  build_node_table( CSurveyShot **shots
                    , unsigned long numshots
                                                                               , unsigned long **shot_fr_temp
                                                                                , unsigned long **shot_to_temp
                          , unsigned long *solv_numnodes);


	int   surv_loop_close(CSurveyShot **big_shots
                   , unsigned long numshots,int iStage);
	void    mark_solved(CNode  **NodeList
                 , unsigned long NumNodes
                 , int mark);

};

