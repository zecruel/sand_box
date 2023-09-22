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
translate([90, 150, 9.5])
rotate([0, -15.45, 270])
cantoneira(38.1, 4.7, 1416);
//
translate([510, 150, 9.5])
rotate([15.45, 0, 180])
cantoneira(38.1, 4.7, 1416);

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
translate([128, 317.7, 634])
rotate([-15.45, 0, 0])
cube([344, 4.7, 51]);
translate([128, 436.6, 1064])
rotate([-15.45, 0, 0])
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
cube([4.7, 158, 51]);
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
cube([4.7, 158, 51]);
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

L1= 3000;
for (dx=[0:L1:2*L1]){
    translate([dx, 0, 0])
    polo_poc();
 }