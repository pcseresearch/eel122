
{
  tdata->Draw("adc[9]>>h(4096,0,4095)");
  cout<<h->Integral(1900,2750)<<endl;
}
