
source /grid/fermiapp/larsoft/products/setup;
export PRODUCTS=/grid/fermiapp/products/lariat:${PRODUCTS};
export PRODUCTS=/lariat/app/users/jbk/nutools/products:${PRODUCTS};

setup git;
setup gitflow;
setup mrb;
setup ninja v1_6_0;

# gotten from my nutools area
setup nutools v1_14_02 -q e7:prof

source /lariat/app/users/jbk/larsoft/localProducts_larsoft_v04_22_00_e7_prof/setup

cd /lariat/app/users/jbk/

# this is not needed, it comes from the development area
# source /lariat/app/users/jbk/lariat/v01_08_02/localProducts_lariatsoft_v01_08_02_e7_prof/setup

# now I can do the develop_lariat

