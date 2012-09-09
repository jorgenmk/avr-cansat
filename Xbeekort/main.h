typedef struct {char FixTime[11];
	char Lat[10];
	char LatDir[2];
	char Lon[11];
	char LonDir[2];
	char FixQuality[2];
	char SatsUsed[3];
	char Alt[11];
	char AltUnit[2];
	char Speed[10];
	char UnitN[2];
	char SpeedKPH[10];
	char UnitK[2];
	char Tcourse[6];
	char TcourseRef[2];
	char Mcourse[6];
	char McourseRef[2];
	char temp_string_in[6];	//!< Contains a string representing the temperature inside the cansat
	char temp_string_out[6];	//!< Contains a string representing the temperature outside the cansat
	char humidity_string[6];	//!< Contains a string representing the humidity level outside the cansat
	char pressure_string[6];	//!< Contains a string representing the pressure outside the cansat
	char accel_string[6];	//!< Contains a string representing the acceleration forces applied to cansat
}data;
