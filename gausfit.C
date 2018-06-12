void gausfit(TTree *T, Double_t &mean, Double_t &sigma){
   //-------------------Create alias with correct numbering----------------------
  // Double_t c;
  //c == 13000.;
  T->SetAlias("c1","13000.");
  T->SetAlias("tdc1","tdc[1]");
  T->SetAlias("tdc2","tdc[2]");
  T->SetAlias("tdc3","tdc[3]");
  T->SetAlias("tdc4","tdc[4]");
  T->SetAlias("tdc5","tdc[5]");
  T->SetAlias("tdc6","tdc[6]");
  T->SetAlias("adc1","adc[1]");
  T->SetAlias("adc2","adc[2]");
  T->SetAlias("adc3","adc[3]");
  T->SetAlias("adc4","adc[4]");
  T->SetAlias("adc5","adc[5]");
  T->SetAlias("adc6","adc[6]");
  T->SetAlias("x1","tdc4-tdc1");
  T->SetAlias("x2","tdc5-tdc2");
  T->SetAlias("x3","tdc6-tdc3");
  T->SetAlias("deltax","x1+x3-2*x2");
  T->SetAlias("tdc1c","tdc[1]-c1/adc1");
  T->SetAlias("tdc2c","tdc[2]-c1/adc2");
  T->SetAlias("tdc3c","tdc[3]-c1/adc3");
  T->SetAlias("tdc4c","tdc[4]-c1/adc4");
  T->SetAlias("tdc5c","tdc[5]-c1/adc5");
  T->SetAlias("tdc6c","tdc[6]-c1/adc6");
  T->SetAlias("t1","(tdc4c+tdc1c)/2");
  T->SetAlias("t2","(tdc5c+tdc2c)/2");
  T->SetAlias("t3","(tdc6c+tdc3c)/2");
  T->SetAlias("t13","t1-t3");
  T->SetAlias("deltat","t2-(t1+t3)/2");

 
  //-------------------Define cuts---------------------------------------------
  TCut cut_t1 = "tdc1>900";
  TCut cut_t2 = "tdc2>900";
  TCut cut_t3 = "tdc3>900";
  TCut cut_t4 = "tdc4>900";
  TCut cut_t5 = "tdc5>900";
  TCut cut_t6 = "tdc6>900";
  TCut cut_tll = cut_t1 && cut_t2 && cut_t3 && cut_t4 && cut_t5 && cut_t6;
  //
  TCut cut_a1 = "adc1>600&&adc1<1200";
  TCut cut_a2 = "adc2>600&&adc2<1200";
  TCut cut_a3 = "adc3>500&&adc3<1200";
  TCut cut_a4 = "adc4>500&&adc4<1200";
  TCut cut_a5 = "adc5>400&&adc5<1200";
  TCut cut_a6 = "adc6>400&&adc6<1100";
  TCut cut_all = cut_a1 && cut_a2 && cut_a3 && cut_a4 && cut_a5 && cut_a6;
  //
  TCut cut_xll = "abs(x2)<40&&abs(x1+70)<40&&abs(x3+22)<40";  
  //TCut cut_xll = "abs(x2-60)<40&&abs(x1+50)<40&&abs(x3-10)<40";
  // T->Draw("deltax","abs(deltax+155)<40"&&cut_xll,"");
  T->Draw("deltat>>h1(400,-100,100)",cut_all&&cut_tll&&"abs(t13+39)<10");
  h1->Fit("gaus","r","",-35,-22);
  mean = gaus->GetParameter(1);
  sigma = gaus->GetParameter(2);
}
