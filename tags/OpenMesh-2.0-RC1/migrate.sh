#!/bin/sh
#
# $Id: migrate.sh,v 1.1.1.1 2006-10-25 16:13:26 habbecke Exp $

if [ "$1" == "-s" ]; then
   shift 1
   show_skipped=1
fi

echo "This script prepares your source for OpenMesh Version 1-0-x"
echo
echo "Warning! This script visits ALL subdirectories of the current"
echo "working directory and"
echo 
echo " 1. changes includes path in text files"
echo " 2. changes any occurence of 'MeshIO' into 'IO'"
echo " 3. changes any occurence of 'DefaultAttributer' into 'Attributes'"
echo " 4. changes any occurence of 'Attributer.hh' into 'Attributes.hh'"
echo " 5. changes any occurence of 'Base::Refs::*' into 'Refs::*'"
echo " 6. changes in all ACGMakefile the OpenMesh library names"
echo ""
echo -n "Proceed [yes/NO]? "
read
if [ -z $REPLY ] || [ $REPLY != "yes" ]; then
   echo "Aborted!"
   exit 0
fi

# ------------------------------------------------------------------------------
# include path 

echo ""
echo "# changing include paths"
TMP=`grep -rl include .`
for I in $TMP ; do 
  if [[ "$I" != *_dbg* ]] && [[ $I != *_max* ]] && \
     [[ "$I" != *_opt* ]] && [[ $I != *_prf* ]] && \
     [[ "$I" != *.so ]]; then

    echo -n "processing $I.."
    # "correct" include path to MeshCheckerT.hh
    # OpenMesh/Tools/MeshCheckerT.hh -> OpenMeshTools/Utils/MeshCheckerT.hh
    if  (grep -q "include.*OpenMesh/Tools/MeshCheckerT.hh" $I); then
       cat $I | \
       perl -pe 's/^(\s*#\s*include\s*<.*)\/Tools\/MeshCheckerT.hh/\1Tools\/Utils\/MeshCheckerT.hh/' > $I.out
       mv $I.out $I
       if [ $? -eq 0 ]; then
         echo -n "."
       fi
    fi

    # correct include path to modules
    if  (grep -q "include.*OpenMesh" $I)       && \
       !(grep -q "include.*OpenMesh/Core" $I)  && \
       !(grep -q "include.*OpenMesh/Tools" $I) && \
       !(grep -q "include.*OpenMesh/Apps" $I); then
      cat $I | \
      perl -pe 's/^(\s*#\s*include\s*<)\s*OpenMesh(\/.*)$/\1OpenMesh\/Core\2/' | \
      perl -pe 's/^(\s*#\s*include\s*<)\s*OpenMeshTools(\/.*)$/\1OpenMesh\/Tools\2/' | \
      perl -pe 's/^(\s*#\s*include\s*<)\s*OpenMeshApps(\/.*)$/\1OpenMesh\/Apps\2/' > $I.out

      mv $I.out $I
      if [ $? -gt 0 ]; then
        echo "ERROR"
      else
        echo "done"
      fi
    elif (($show_skipped)); then
      echo "skipped"
    else
      echo -ne "\r                                                               \r"
    fi
    
  fi
done

# ------------------------------------------------------------------------------
echo "# changing namespaces"
TMP=`grep -rl MeshIO .`
for I in $TMP ; do 
  if [[ "$I" != *_dbg* ]] && [[ $I != *_max* ]] && \
     [[ "$I" != *_opt* ]] && [[ $I != *_prf* ]] && \
     [[ "$I" != *.so ]]; then
     
    echo -n "processing $I.."
    if (grep -qE "MeshIO[^\.h]" $I); then
      cat $I | \
      perl -pe 's/MeshIO([^\.h])/IO\1/' > $I.out
      mv $I.out $I
      if [ $? -gt 0 ]; then
        echo "ERROR"
      else
        echo "done"
      fi
    elif (($show_skipped)); then
      echo "skipped"
    else
      echo -ne "\r                                                               \r"
    fi
  fi
done

# ------------------------------------------------------------------------------
echo "# Change DefaultAttributer to Attributes"
TMP=`grep -rl DefaultAttributer .`
for I in $TMP ; do 
  if [[ "$I" != *_dbg* ]] && [[ $I != *_max* ]] && \
     [[ "$I" != *_opt* ]] && [[ $I != *_prf* ]] && \
     [[ "$I" != *.so ]]; then
     
    echo -n "processing $I.."
    if (grep -qE "DefaultAttributer" $I); then
      cat $I | \
      perl -pe 's/DefaultAttributer/Attributes/' > $I.out
      mv $I.out $I
      if [ $? -gt 0 ]; then
        echo "ERROR"
      else
        echo "done"
      fi
    elif (($show_skipped)); then
      echo "skipped"
    else
      echo -ne "\r                                                               \r"
    fi
  fi
done

# ------------------------------------------------------------------------------
echo "# Change Attributer.hh to Attributes.hh"
TMP=`grep -rl Attributer.hh .`
for I in $TMP ; do 
  if [[ "$I" != *_dbg* ]] && [[ $I != *_max* ]] && \
     [[ "$I" != *_opt* ]] && [[ $I != *_prf* ]] && \
     [[ "$I" != *.so ]]; then
     
    echo -n "processing $I.."
    if (grep -q "Attributer.hh" $I); then
      cat $I | \
      perl -pe 's/Attributer\.hh/Attributes.hh/' > $I.out
      mv $I.out $I
      if [ $? -gt 0 ]; then
        echo "ERROR"
      else
        echo "done"
      fi
    elif (($show_skipped)); then
      echo "skipped"
    else
      echo -ne "\r                                                               \r"
    fi
  fi
done

# ------------------------------------------------------------------------------
echo "# Change Base::Refs::* to Refs::*"
TMP=`grep -rl Base::Refs:: .`
for I in $TMP ; do 
  if [[ "$I" != *_dbg* ]] && [[ $I != *_max* ]] && \
     [[ "$I" != *_opt* ]] && [[ $I != *_prf* ]] && \
     [[ "$I" != *.so ]]; then
     
    echo -n "processing $I.."
    if (grep -q "Base::Refs::" $I); then
      cat $I | \
      perl -pe 's/Base::Refs::/Refs::/' > $I.out
      mv $I.out $I
      if [ $? -gt 0 ]; then
        echo "ERROR"
      else
        echo "done"
      fi
    elif (($show_skipped)); then
      echo "skipped"
    else
      echo -ne "\r                                                               \r"
    fi
  fi
done

# ------------------------------------------------------------------------------
echo "# changing ACGMakefiles"
for I in `find . -name 'ACGMakefile'`; do
  if [[ "$I" != *_dbg* ]] && [[ $I != *_max* ]] && \
     [[ "$I" != *_opt* ]] && [[ $I != *_prf* ]] && \
     [[ "$I" != *.so ]]; then

     echo -n "processing $I.."
     if  (grep -q ".*OpenMesh" $I)      && \
        !(grep -q ".*OpenMesh/Core" $I) && \
        !(grep -q ".*OpenMesh/Apps" $I); then
       cat $I | \
       perl -pe 's/OpenMesh/OpenMesh\/Core/' | \
       perl -pe 's/OpenMeshApps/OpenMesh\/Apps/' > $I.out
       mv $I.out $I
       if [ $? -gt 0 ]; then
         echo "ERROR"
       else
         echo "done";
       fi
     elif (($show_skipped)); then
       echo "skipped"
     else
       echo -ne "\r                                                               \r"
     fi
  fi
done


