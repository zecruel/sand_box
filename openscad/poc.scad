module cantoneira
(aba = 30, esp = 3, compr = 1000){

linear_extrude(height = compr) polygon( points=[[0,0],[aba,0],[aba,esp],[esp,esp],[esp,aba],[0,aba]] );
    
}
module viga_u
(base = 50, aba = 30, esp = 3, compr = 1000){

linear_extrude(height = compr) polygon( points=[[0,0],[0,aba],
    [base,aba], [base,0],
    [base-esp,0],[base-esp,aba-esp],
    [esp,aba-esp], [esp,0]]
    );
    
}


module pe_poc(){
    
    //chapa do pe
    cube([600, 200, 9.5]);
    translate([110, 62, 9.5])
    cube([4.7, 38, 100]);
    translate([486, 62, 9.5])
    cube([4.7, 38, 100]);
    translate([52, 115, 9.5])
    cube([38, 4.7, 100]);
    translate([510, 115, 9.5])
    cube([38, 4.7, 100]);
    translate([550, 130, 9.5])
    cylinder(30, 9, 9);
    translate([50, 130, 9.5])
    cylinder(30, 9, 9);
    
    //maior
    translate([90, 100, 9.5])
    cantoneira(38.1, 4.7, 2694);
    //
    translate([510, 100, 9.5])
    rotate([0, 0, 90])
    cantoneira(38.1, 4.7, 2694);
    
    //inclinado
    translate([90, 150, 0])
    rotate([0, -15.4, 270])
    cantoneira(38.1, 4.7, 1419.5);
    //
    
    translate([510, 150, 0])
    rotate([15.4, 0, 180])
    cantoneira(38.1, 4.7, 1419.5);
    
    //menor
    translate([90, 527, 1368.5])
    rotate([0, 0, 270])
    cantoneira(38.1, 4.7, 1335);
    //
    translate([510, 527, 1368.5])
    rotate([0, 0, 180])
    cantoneira(38.1, 4.7, 1335);
    
    //topo
    translate([90, 100, 2703.5])
    rotate([270, 0, 0])
    cantoneira(50.8, 4.7, 427);
    //
    translate([510, 100, 2703.5])
    rotate([0, 90, 90])
    cantoneira(50.8, 4.7, 427);
    
    translate([90, 259.2, 2703.5])
    rotate([90, 0, 90])
    viga_u(101.6, 41.3, 4.7, 420);
    translate([117, 310, 2690])
    cylinder(70, 8, 8);
    translate([483, 310, 2690])
    cylinder(70, 8, 8);
    
    //costelas
    translate([128, 100, 9.5])
    cube([344, 4.7, 51]);
    translate([128, 100, 634])
    cube([344, 4.7, 51]);
    translate([128, 100, 1064])
    cube([344, 4.7, 51]);
    translate([128, 100, 1369])
    cube([344, 4.7, 51]);
    translate([128, 100, 1719])
    cube([344, 4.7, 51]);
    translate([128, 100, 2074])
    cube([344, 4.7, 51]);
    translate([128, 100, 2268])
    cube([344, 4.7, 51]);
    translate([128, 100, 2462])
    cube([344, 4.7, 51]);
    translate([128, 100, 2652])
    cube([344, 4.7, 51]);
    
    //
    translate([128, 319.8, 634])
    rotate([-15.4, 0, 0])
    cube([344, 4.7, 51]);
    translate([128, 438.2, 1064])
    rotate([-15.4, 0, 0])
    cube([344, 4.7, 51]);
    translate([128, 522.3, 1369])
    cube([344, 4.7, 51]);
    translate([128, 522.3, 1719])
    cube([344, 4.7, 51]);
    translate([128, 522.3, 2074])
    cube([344, 4.7, 51]);
    translate([128, 522.3, 2268])
    cube([344, 4.7, 51]);
    translate([128, 522.3, 2462])
    cube([344, 4.7, 51]);
    translate([128, 522.3, 2652])
    cube([344, 4.7, 51]);
    
    translate([90, 138.1, 634])
    cube([4.7, 162, 51]);
    translate([90, 138.1, 1064])
    cube([4.7, 279, 51]);
    translate([90, 138.1, 1369])
    cube([4.7, 351, 51]);
    translate([90, 138.1, 1719])
    cube([4.7, 351, 51]);
    translate([90, 138.1, 2074])
    cube([4.7, 351, 51]);
    translate([90, 138.1, 2268])
    cube([4.7, 351, 51]);
    translate([90, 138.1, 2462])
    cube([4.7, 351, 51]);
    
    
    translate([505.3, 138.1, 634])
    cube([4.7, 162, 51]);
    translate([505.3, 138.1, 1064])
    cube([4.7, 279, 51]);
    translate([505.3, 138.1, 1369])
    cube([4.7, 351, 51]);
    translate([505.3, 138.1, 1719])
    cube([4.7, 351, 51]);
    translate([505.3, 138.1, 2074])
    cube([4.7, 351, 51]);
    translate([505.3, 138.1, 2268])
    cube([4.7, 351, 51]);
    translate([505.3, 138.1, 2462])
    cube([4.7, 351, 51]);
}


module polo_poc
(L2 = 1500){
    L3 = L2 - 730;
    pe_poc();
    translate([0, L2+200, 0])
    mirror([0,1,0]) pe_poc();
    
    translate([90, 465, 2703.5])
    rotate([270, 90, 0])
    cantoneira(50.8, 4.7, L3);
    
    translate([70, 483, 2675])
    rotate([0, 90, 0])
    cylinder(40, 8, 8);
    
    translate([70, 440+L3, 2675])
    rotate([0, 90, 0])
    cylinder(40, 8, 8);
    
    translate([90, 465, 1420])
    rotate([270, 90, 0])
    cantoneira(50.8, 4.7, L3);
        
    translate([70, 483, 1395])
    rotate([0, 90, 0])
    cylinder(40, 8, 8);
    
    translate([70, 440+L3, 1395])
    rotate([0, 90, 0])
    cylinder(40, 8, 8);
    
    
    mirror([1,0,0]) 
    translate([-600, 0, 0])
    union(){
        translate([90, 465, 2703.5])
        rotate([270, 90, 0])
        cantoneira(50.8, 4.7, L3);
        
        translate([70, 483, 2675])
        rotate([0, 90, 0])
        cylinder(40, 8, 8);
        
        translate([70, 440+L3, 2675])
        rotate([0, 90, 0])
        cylinder(40, 8, 8);
        
        translate([90, 465, 1420])
        rotate([270, 90, 0])
        cantoneira(50.8, 4.7, L3);
            
        translate([70, 483, 1395])
        rotate([0, 90, 0])
        cylinder(40, 8, 8);
        
        translate([70, 440+L3, 1395])
        rotate([0, 90, 0])
        cylinder(40, 8, 8);
    }
}


module poc_par (
    L1= 3000,
    L2 = 1500){
    L4 = L1 - 334;
    for (dx=[0,L1]){
        translate([dx, 0, 0])
        polo_poc(L2);
     
        translate([dx+470, 100, 2703.5])
        rotate([90, 180, 90])
        cantoneira(50.8, 4.7, L4);
         
        translate([dx+490, 120, 2675])
        rotate([90, 0, 0])
        cylinder(40, 8, 8);
        
        translate([dx+450+L4, 120, 2675])
        rotate([90, 0, 0])
        cylinder(40, 8, 8);
         
        translate([dx+470, L2+100, 2703.5])
        rotate([90, 90, 90])
        cantoneira(50.8, 4.7, L4);
         
        translate([dx+490, L2+120, 2675])
        rotate([90, 0, 0])
        cylinder(40, 8, 8);
        
        translate([dx+450+L4, L2+120, 2675])
        rotate([90, 0, 0])
        cylinder(40, 8, 8);
    
    }
    
    translate([2*L1, 0, 0])
    polo_poc(L2);
}

module isolador(h=1000,
r = 130){
    n = floor(h / (0.54*r));
    for(dz = [0:0.54*r:(n-1)*0.54*r]){
        translate([0, 0, dz])
        cylinder(0.23*r, 0.69*r, 0.69*r);
        translate([0, 0, dz+0.23*r])
        cylinder(0.08*r, r, r);
        translate([0, 0, dz+0.31*r])
        cylinder(0.23*r, r, 0.69*r);
    }
    s = h-(n*0.54*r);
    translate([0, 0, n*0.54*r])
    cylinder(s, 0.69*r, 0.69*r);
}
module polo_term (
//color("DarkGray") 
//poc_par();
r = 130,
h = 1200,
c = 1500
){
color("DarkGray")
translate([0, 0.125*c, 0])
cylinder(0.3*r, 0.4*r, 0.4*r);

color("DarkGray")
translate([0, 0.125*c, 0.3*r])
cylinder(0.3*r, 0.69*r, 0.69*r);

color("SaddleBrown")
translate([0, 0.125*c, 0.6*r])
isolador(h-1.8*r,r);

color("DarkGray")
translate([0, 0.125*c, h-1.2*r])
cylinder(0.3*r, 0.69*r, 0.69*r);

color("DarkGray")
translate([0, 0.125*c, h-0.9*r])
cylinder(0.3*r, 0.4*r, 0.4*r);

color("DarkGray")
translate([-0.4*r, 0.07*c, h-0.6*r])
cube([0.8*r, 0.09*c, 0.3*r]);
color("DarkGray")
translate([-0.4*r, 0, h-0.3*r])
cube([0.8*r, 0.1*c, 0.3*r]);
}

module sec_horiz_polo (
r = 130,
h = 1200,
c = 1500){
    
    translate([100, 0, 0])
    rotate([-90, 180, 0])
    color("LightSlateGray")
    viga_u(200, 100, 5, c);
    

    translate([0, 0, 100])
    polo_term(r, h-100, c);

    translate([0, c, 100])
    rotate([0, 0, 180])
    polo_term(r, h-100, c);
    
    
    //lamina
    color("DarkGray")
    translate([0, 0.12*c, h])
    rotate([271, 0, 0])
    cylinder(0.75*c, 0.3*r, 0.3*r);
    
    color("DarkGray")
    translate([0.27*r, 0.105*c, h-0.3*r])
    cube([0.13*r, 0.07*c, 0.7*r]);
    
    color("DarkGray")
    translate([-0.4*r, 0.105*c, h-0.3*r])
    cube([0.13*r, 0.07*c, 0.7*r]);
    
    color("DarkGray")
    translate([0.27*r, 0.825*c, h-0.3*r])
    cube([0.13*r, 0.07*c, 0.9*r]);
    color("DarkGray")
    translate([-0.4*r, 0.825*c, h-0.3*r])
    cube([0.13*r, 0.07*c, 0.9*r]);
    
}
color("Gray")
poc_par(2500);
translate([300, -100, 2744.7])
sec_horiz_polo(130, 1835, 1960);

translate([300+2500, -100, 2744.7])
sec_horiz_polo(130, 1835, 1960);

translate([300+5000, -100, 2744.7])
sec_horiz_polo(130, 1835, 1960);