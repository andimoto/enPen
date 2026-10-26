$fn=80;
extra = 0.02;

simRod = false;

upperVariant = 2;
lowerVariant = 1;

th_x = 13.5;
th_y = 45;
th_z = 50;

slideh_x = 15;
slideh_z = 5;
slideChampf = 2;
slideh_xMov =1;

enableInserts = false;

screwM3 = 3;
screwTolerance = 0.1;
screwM3HeadDia = 6;
screwM3HeadZ = 3;

ScrewInsertM3 = 4.5;

function screwOrInsert() = 
    (enableInserts == true) ? ScrewInsertM3 : screwM3;

th_adapter_z = 55;
th_adapter_y1 = 29.5;
th_adapter_y2 = 25.5;
th_adapter_holeMoveZ = 2.5;
th_adapter_x1 = 2;
th_adapter_x2 = 4;
th_adapter_x3 = 1;
th_adapter_yMove = (th_adapter_y1 - th_adapter_y2)/2;

attiny_y = 13;
attiny_x = 2.4;
attiny_z = 8;

servoCutX = 12.5;
servoCutY = 32;
servoCutZ = 23;
servoCutPTh = 3;
servoCutPZ = 5;

servoScrew = 2;
servoScrewLen = 5;


screwM3Slide = 3.2;
slideBearingM3 = 4;
slideB3Z = 5;
screwM4Slide = 4.1;
screwM4HeadDia = 7.1;
screwM4HeadZ = 4;
slideBearingM4 = 6;
slideB4Z = 5;
hexHoleDia = 6.25;

hexSlide = false;
enableM3slide = false;
enableM3slideBearing = false;
enableM4slide = true;
enableM4slideBearing = false;

runAssert();

function getFn() = (hexSlide) ? 6: 80;
function getDia() = (hexSlide) ? (hexHoleDia) : (getSlideDia());
function getSlideDia() = (enableM4slide) ? (screwM4Slide) : (screwM3Slide);
// function getSlideHolderZ() = (enableM4slide) ? (10) : (5);
function getHeadDia() = (enableM4slide) ? (screwM4HeadDia) : (screwM3HeadDia);
function getHeadZ() = (enableM4slide) ? (screwM4HeadZ) : (screwM3HeadZ);



// toolheadAdapter();
module toolheadAdapter()
{
    difference()
    {
        union()
        {
            cube([th_adapter_x1,th_adapter_y1,th_adapter_z]);
            hull()
            {
                translate([th_adapter_x1-extra,0,0]) 
                cube([extra,th_adapter_y1,th_adapter_z]);

                translate([th_adapter_x1+th_adapter_x2,th_adapter_yMove,0]) 
                cube([extra,th_adapter_y2,th_adapter_z]);
            }
            translate([th_adapter_x1+th_adapter_x2,th_adapter_yMove,0]) 
                cube([th_adapter_x3,th_adapter_y2,th_adapter_z]);
        }

        translate([0,th_adapter_y1/2-th_y/2,th_adapter_holeMoveZ]) 
        union()
        {
            translate([-extra,th_y/2,slideh_z/2]) 
            rotate([0,90,0])
            cylinder(h = th_x+extra*2, d = screwOrInsert());

            translate([-extra,th_y/2,th_z-slideh_z*2]) 
            rotate([0,90,0])
            cylinder(h = th_x+extra*2, d = screwOrInsert());
            // translate([-extra,th_y/3,th_z-slideh_z*2]) 
            // rotate([0,90,0])
            // cylinder(h = th_x+extra*2, d = screwOrInsert());
        }

        temp_adapterXMove = th_adapter_x1+th_adapter_x2 + th_adapter_x3;
        translate([temp_adapterXMove-attiny_x+extra,th_adapter_y1/2-attiny_y/2,
        th_adapter_z-attiny_z-2+extra]) 
        cube([attiny_x,attiny_y,attiny_z+2]);
    }
}

if(simRod == true)
{
color("Silver")
union()
{
translate([th_x+slideh_xMov + slideh_x/2,(th_y/3)/2,-extra]) 
            cylinder(h = th_z+extra*2, d = screwM3);
translate([th_x+slideh_xMov + slideh_x/2,5*(th_y/3)/2,-extra]) 
            cylinder(h = th_z+extra*2, d = screwM3);
}
}


penToolhead();
module penToolhead()
{
    difference() 
    {
        union()
        {
            cube([th_x,th_y,th_z]);

            translate([th_x,0,0]) 
            slideHolder(lowerVariant);

            translate([th_x,2*(th_y/3),0]) 
            slideHolder(lowerVariant);

            translate([th_x,(th_y/3),th_z]) 
            rotate([180,0,0]) 
            slideHolder(upperVariant,5);

            translate([th_x,3*(th_y/3),th_z]) 
            rotate([180,0,0]) 
            slideHolder(upperVariant,5);
        }

        translate([1,-3,1]) 
        servoFootprint();

        translate([-extra,th_y/2,slideh_z/2]) 
        rotate([0,90,0])
        cylinder(h = th_x+extra*2, d = screwM3+screwTolerance);

        translate([-extra,th_y/2,th_z-slideh_z*2]) 
        rotate([0,90,0])
        cylinder(h = th_x+extra*2, d = screwM3+screwTolerance);
        // translate([-extra,th_y/3,th_z-slideh_z*2]) 
        // rotate([0,90,0])
        // cylinder(h = th_x+extra*2, d = screwM3+screwTolerance);

        // screws at the bottom for rubber
        translate([th_x+screwM3/2,(th_y/3)/2,-extra]) 
            cylinder(h = slideh_z-2+extra*2, d = screwM3);
        translate([th_x+screwM3/2,5*(th_y/3)/2,-extra]) 
            cylinder(h = slideh_z-2+extra*2, d = screwM3);

        // top screws for mounting things to it.
        translate([th_x/2,(th_y/3)/2,th_z+extra-slideh_z]) 
            cylinder(h = slideh_z+extra*2, d = screwM3);
        translate([th_x/2,5*(th_y/3)/2,th_z-slideh_z]) 
            cylinder(h = slideh_z+extra*2, d = screwM3);

    }

}


// translate([th_x+3,0,slideh_z+2])
// penHolder();
module penHolder()
{
    difference()
    {
        union()
        {
            hull()
            {
                cube([12-slideChampf,th_y,20]);
                translate([12,slideChampf,0])
                    cube([extra,th_y-slideChampf*2,20]);
        
            }

            translate([17,th_y/2,0])
            cylinder(d=20,h=20);
        }

        translate([17,th_y/2,-extra])
            cylinder(d=13,h=20+extra*2);

        translate([17-extra,th_y/2,20/4]) 
        rotate([0,90,0])
        cylinder(h = th_x+extra*2, d = screwM3);
        translate([17-extra,th_y/2,20/4*3]) 
        rotate([0,90,0])
        cylinder(h = th_x+extra*2, d = screwM3);

        translate([0,th_y/3,-extra])
            cube([5,th_y/3,10]);

        //slide rods
        translate([-0.5+12/2,th_y/3/2,-extra])
            rotate([0,0,30])
            cylinder(d=getDia(),h=20+extra*2, $fn=getFn());
        translate([-0.5+12/2,th_y-th_y/3/2,-extra])
            rotate([0,0,30])
            cylinder(d=getDia(),h=20+extra*2, $fn=getFn());

        if(enableM3slideBearing == true)
        {
            translate([-0.5+12/2,th_y/3/2,-extra])
            rotate([0,0,30])
            union(){
                cylinder(d=slideBearingM3,h=slideB3Z+extra*2, $fn=getFn());
                translate([0,0,20-5])
                cylinder(d=slideBearingM3,h=slideB3Z+extra*2, $fn=getFn());
            }
            
            translate([-0.5+12/2,th_y-th_y/3/2,-extra])
            rotate([0,0,30])
            union(){
                cylinder(d=slideBearingM3,h=slideB3Z+extra*2, $fn=getFn());
                translate([0,0,20-5])
                cylinder(d=slideBearingM3,h=slideB3Z+extra*2, $fn=getFn());
            }
        }

        if(enableM4slideBearing == true)
        {
            translate([-0.5+12/2,th_y/3/2,-extra])
            rotate([0,0,30])
            union(){
                cylinder(d=slideBearingM4,h=slideB4Z+extra*2, $fn=getFn());
                translate([0,0,20-5])
                cylinder(d=slideBearingM4,h=slideB4Z+extra*2, $fn=getFn());
            }
            
            translate([-0.5+12/2,th_y-th_y/3/2,-extra])
            rotate([0,0,30])
            union(){
                cylinder(d=slideBearingM4,h=slideB4Z+extra*2, $fn=getFn());
                translate([0,0,20-5])
                cylinder(d=slideBearingM4,h=slideB4Z+extra*2, $fn=getFn());
            }
        }


        translate([12/2,-extra,20/4*3]) 
        rotate([-90,00,0])
        cylinder(h = 3, d = screwM3);

        translate([12/2,th_y+extra,20/4*3]) 
        rotate([90,00,0])
        cylinder(h = 3, d = screwM3);
    }
}

// slideHolder(variant = 2, thick=5);
module slideHolder(variant = 1, thick=5)
{
    difference()
    {
        hull()
        {
            cube([slideh_x-slideChampf,th_y/3,thick]);
            translate([slideh_x-slideChampf,slideChampf,0]) 
            cube([slideChampf,(th_y/3)-slideChampf*2,thick]);
        }

        if(variant == 1)
        {
            translate([slideh_xMov + slideh_x/2,(th_y/3)/2,-extra]) 
            cylinder(h = slideh_z+extra*2, d = getSlideDia()+screwTolerance);
            translate([slideh_xMov + slideh_x/2,(th_y/3)/2,-extra]) 
            cylinder(h = screwM3HeadZ, d = getHeadDia());
        }
        if(variant == 2)
        {
            translate([slideh_xMov + slideh_x/2,(th_y/3)/2,0]) 
            hull()
            {
                cylinder(h = thick+extra*2, d = getSlideDia()+screwTolerance);
                translate([slideh_x,0,-extra])
                cylinder(h = thick+extra*2, d = getSlideDia()+screwTolerance);
            }
            
            // translate([slideh_xMov + slideh_x/2,(th_y/3)/2,0]) 
            // hull()
            // {
            //     cylinder(h = thick-2+extra, d = getHeadDia(), $fn=6);
            //     translate([slideh_x,0,-extra]) 
            //     cylinder(h = thick-2+extra, d = getHeadDia(), $fn=6);
            // }
        }

    }
}



// servoFootprint();
module servoFootprint()
{
    translate([0,0,slideh_z+servoCutPZ]) 
    cube([servoCutX+extra,servoCutY,servoCutZ]);
    translate([0,18-servoCutPTh,slideh_z]) 
    cube([servoCutX+extra,servoCutPTh,servoCutZ +servoCutPZ*2]);

    translate([0,0,slideh_z-1]) 
    cube([servoCutX+extra,18-servoCutPTh,screwM3HeadDia]);
    translate([0,0,servoCutZ+servoCutPZ*2]) 
    cube([servoCutX+extra,18-servoCutPTh,screwM3HeadDia]);


    #translate([servoCutX/2,20-servoCutPTh,slideh_z-0.5+(slideh_z/2)]) 
    rotate([-90,0,0]) 
    cylinder(h = servoScrewLen, d = servoScrew);

    translate([servoCutX/2,20-servoCutPTh,slideh_z+0.5+servoCutZ +servoCutPZ*2 -(servoCutPZ/2)]) 
    rotate([-90,0,0]) 
    cylinder(h = servoScrewLen, d = servoScrew);
}


module runAssert()
{
    if(hexSlide == true)
    {
        assert(!enableM3slide);
        assert(!enableM3slideBearing);
        assert(!enableM4slide);
        assert(!enableM4slideBearing);
    }
    if(enableM3slide == true)
    {
        assert(!hexSlide);
        assert(!enableM4slide);
        assert(!enableM4slideBearing);
    }else{
        assert(!enableM3slideBearing);
    }

    if(enableM4slide == true)
    {
        assert(!hexSlide);
        assert(!enableM3slide);
        assert(!enableM3slideBearing);
    }else {  
        assert(!enableM4slideBearing);
    }
}