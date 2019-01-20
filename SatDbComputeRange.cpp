void SatDb::getSatParam(SatID sat, DayTime& gpstime ,const YumaAlmanacStore& yums)

    {

	 satDb_ostruct temp_sat;
	 Position satR,satV; // can be reduced to ECEF or Triple
	 try
	 {

     AlmOrbit a = yums.findAlmanac(sat, gpstime);
	 sv_xvt     = a.svXvt(gpstime);
	 short week = a.getFullWeek();
	 // Make the output data structure

	 /*these param is needed for carr phase calculation*/

	 //temp_sat.toa = a.getToaTime(); this is wrong value
	 temp_sat.af0 = a.getAF0 ();
	 temp_sat.af1 = a.getAF1 ();

	 double time_elap = gpstime - temp_sat.toa;
	 temp_sat.prn=sat;
	 temp_sat.sathealth=!(a.getSVHealth());


	 temp_sat.satXvt=sv_xvt;

	 long temptoaSow = a.getToaSOW();
	 short tempweek =a.getToaWeek();

	 DayTime correctToa(0.L);
	 correctToa.setGPS(tempweek,(double)temptoaSow);
	 double time_elap1 = gpstime - correctToa;

 	 temp_sat.toa = correctToa;

	 satR.setECEF(sv_xvt.x[M_ZERO],sv_xvt.x[M_ONE],sv_xvt.x[M_TWO]);
	 satV.setECEF(sv_xvt.v[M_ZERO],sv_xvt.v[M_ONE],sv_xvt.v[M_TWO]);

    // Relativity Correction
	// Correction = -2*dot(R,V)/(c*c) ,
	// R & V are position and velocity vector

	 temp_sat.relativity = -2*(satR.X()/C_GPS_M)*(satV.X()/C_GPS_M)
             -2*(satR.Y()/C_GPS_M)*(satV.Y()/C_GPS_M)
             -2*(satR.Z()/C_GPS_M)*(satV.Z()/C_GPS_M);


	 // This take care of second roll over and other time adjustment
	 // as addition operator is overloaded with "addLongDeltaTime" function
	 // of gpstk::DayTime class.
	// correction = af0+af1(gpstime - toc of almanac) + relativity
	// not adding temp_sat.af0 as its the bias term

	 double correction =   time_elap1 * temp_sat.af1 + temp_sat.relativity; // correction with relativity , as in Yuma file it doesnt come
	 temp_sat.satTime = gpstime + correction;
	 //stats[sat].clock.Add(correction);
	 temp_sat.clkCorrection = correction;//a.getAF1()*C_GPS_M;

 // Take care of af0 and af1 of the clock and generate all frequency
	// sat correction is calculated in Satdb engine
	// Orginal Sat clock freq is offset for relativistic correction
	 temp_sat.satOscFreq 	   	= 	RSVCLK + RSVCLK * correction;
	 temp_sat.carrFreqL1 		=	temp_sat.satOscFreq*L1_MULT;
	 temp_sat.carrFreqL2 		=	temp_sat.satOscFreq*L2_MULT;
	 temp_sat.carrFreqL5 		=	temp_sat.satOscFreq*115;
	 temp_sat.chipFreqCA		=   temp_sat.satOscFreq / 10;
	 temp_sat.chipFreqPY		=   time_elap1;





	 // Almorbit satpos computation doesnt add relativity and ddtime param is empty, using it to do relativistic correction
	 temp_sat.satXvt.ddtime = correction; // af0+af1(gpstime - toc of almanac) + relativity

	 if(!temp_sat.sathealth)
	 {temp_sat.dataValid = false;
	 }

	 satDb_pData.push_back(temp_sat);
	 satpData[sat]=temp_sat;

}
	  catch(InvalidRequest &e)
	 {
		 temp_sat.prn=sat;
		 temp_sat.sathealth=false;
		 temp_sat.satXvt.x[M_ZERO] = (double)M_ZERO;
		 temp_sat.satXvt.x[M_ONE] = (double)M_ZERO;
		 temp_sat.satXvt.x[M_TWO] = (double)M_ZERO;
	     temp_sat.satXvt.v[M_ZERO] = (double)M_ZERO;
		 temp_sat.satXvt.v[M_ONE] = (double)M_ZERO;
		 temp_sat.satXvt.v[M_TWO] = (double)M_ZERO;
		 temp_sat.af0=M_ZERO;
		  temp_sat.af1=M_ZERO;
		 temp_sat.relativity=M_ZERO;
		 temp_sat.clkCorrection=(double)M_ZERO;
		 satDb_pData.push_back(temp_sat);
		  satpData[sat]=temp_sat;
		  //temp_sat.sattime=0;
	  }


 }