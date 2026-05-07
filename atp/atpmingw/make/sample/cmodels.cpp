extern "C" {
  void  outsix_( char *, int *);
  double cfun_(double arg[]);
  void c_sample_m__(double xdata_ar[],
                    double xin_ar[],
                    double xout_ar[],
                    double xvar_ar[]);
    void c_sample_i__(double xdata_ar[],
                    double xin_ar[],
                    double xout_ar[],
                    double xvar_ar[]);
}

    double cfun_(double arg[])
    {
      char* text = "Executing model 'sample_c++_function'.";
      int len = strlen(text);
      outsix_(text, &len);
      arg[0] = arg[0] + arg[1];
      arg[1] = 10 * arg[0];
      return 2;
    }
        
    void c_sample_m__(double xdata_ar[],
                    double xin_ar[],
                    double xout_ar[],
                    double xvar_ar[])
    { short i;
      char* text = "Executing model 'sample_c++_model'.";
      int len = strlen(text);
      outsix_(text, &len);
      for (i=0; i<xdata_ar[0]; i++)
      { xvar_ar[i] = (2*xvar_ar[i] + xin_ar[i])/2;
        xout_ar[i] = xvar_ar[i] + 100.0;
      }
      return;
    }
        
    void c_sample_i__(double xdata_ar[],
                    double xin_ar[],
                    double xout_ar[],
                    double xvar_ar[])
    { char* text = "Initializing model 'sample_c++_model'.";
      int len = strlen(text);
      outsix_(text, &len);
      return;
    }
 