
/* The minimum Elevation angle */
# define MIN_ELV_ANGLE			1
# define M_ZERO			0
# define M_ONE	1
# define M_TWO 2


/* GPS Theory and Application , Parkinson & Spliker Vol1 : Pg 453*/

# define MAX_REL_DELAY			(double)7.0e-10 //sec
# define MAX_IONO_DELAY			(double)15.0	// meters
# define MAX_TROPO_DELAY		(double)2.4     // meters
# define MAX_SATCLK_ERROR_1SIGMA (double)2.0
# define GLONASS_CODE_SIZE 			511
# define CA_CODE_LENGTH				1023
# define NO_OF_CODEBLOCKS_IN_A_SEC 	1023000
# define CARR_CYCLCE_RESOLUTION  	64