develop_lariat()
{
    # first source the larsoft product area and setup mrb, git and git flow
    # also add the LArIAT products area to the $PRODUCT variable
    source /grid/fermiapp/larsoft/products/setup
    export PRODUCTS=/grid/fermiapp/products/lariat:${PRODUCTS}
    setup git
    setup gitflow
    setup mrb
    setup ninja v1_6_0

    # define the necessary mrb environmental variables and some useful LArIAT related ones
    export MRB_PROJECT=lariatsoft

    #!!!!!!!!----------Change the next line to point to where you want to install lariatsoft-------!!!!!!!!!!!!
    export LARIATDEVDIR=/lariat/app/users/jbk/lariat
    if [ -d "$LARIATDEVDIR" ]; then
        cd ${LARIATDEVDIR}
    else
        echo "LARIATDEVDIR is set to ${LARIATDEVDIR}, which does not exist. " 
        echo "Please set LARIATDEVDIR to the location where you wish to develop lariatsoft" 
        echo "and edit the develop_lariat function in your .bash_profile appropriately" 
        return 0
    fi

    # check if any arguments were given to the function.
    # the arguments are
    # 1: version in format of either "develop" or XX_YY_ZZ
    # 2: Base qualifier, ie eX, depends on the version of art
    # 3: Build qualifier, either debug or prof
    if [ -z $1 ] ; then
        version="develop" 
        ebase="e7" 
        build="prof" 
    else
        version="${1}" 
        ebase="${2}" 
        build="${3}" 
    fi

    # define more variables for convenience based on the input arguments
    export LOCAL_LARIAT=${version}
    export LOCAL_PRODUCTDIR=localProducts_${MRB_PROJECT}_${version}_${ebase}_${build}

    # check that the local lariat area is there, if not,
    # this is a new version to check out so do that
    if [ -d "$LOCAL_LARIAT" ]; then
        cd ${LOCAL_LARIAT}
        source ${LOCAL_PRODUCTDIR}/setup
        # comment out the next 3 lines if you do not want to update your copy of the
        # repository with each log in
        echo "-------------------Updating git------------------" 
        cd srcs/lariatsoft
        git pull origin develop
        echo "-------------------------------------------------" 
        mrbsetenv
    else
        mkdir ${LOCAL_LARIAT}
        cd ${LOCAL_LARIAT}
        mrb newDev -v ${version} -q ${ebase}:${build}
        source ${LOCAL_PRODUCTDIR}/setup
        # Comment out the lines to check out the repository if you don't want to
        # do that with each log in
        echo "-----------Checking Repository Out-----------------" 
        cd srcs/
        mrb gitCheckout ssh://p-lardbt@cdcvs.fnal.gov/cvs/projects/lariatsoft
        # Comment out the lines to build the code if you don't want to
        # do that with each log in
        echo "---------------Building Code----------------------" 
        ### !!!!!!!! Change slf6 to slf5 if using that OS !!!!!!!!! ###
        cd ${LARIATDEVDIR}/${LOCAL_LARIAT}/build_slf6.x86_64
        mrbsetenv
        mrb i --generator ninja
    fi

    #setup lariatsoft

    echo "-------------------------------------------------" 
    echo "Ready to develop LArIATSoft version ${LARIATSOFT_VERSION} in ${LARIATDEVDIR}/${LOCAL_LARIAT}" 
    echo " " 
    echo "When you want to run a job, please setup the ups product by doing" 
    echo "setup lariatsoft ${LARIATSOFT_VERSION} -q ${ebase}:${build}" 
    echo "-------------------------------------------------" 
}
