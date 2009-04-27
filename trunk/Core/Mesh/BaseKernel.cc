/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2003 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *                                License                                    *
 *                                                                           *
 *  This library is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU Lesser General Public License as published    *
 *  by the Free Software Foundation, version 2.1.                            *
 *                                                                           *
 *  This library is distributed in the hope that it will be useful, but      *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *  Lesser General Public License for more details.                          *
 *                                                                           *
 *  You should have received a copy of the GNU Lesser General Public         *
 *  License along with this library; if not, write to the Free Software      *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *                                                                           *
\*===========================================================================*/

#include <OpenMesh/Core/Mesh/BaseKernel.hh>
namespace OpenMesh
{

void BaseKernel::property_stats(std::ostream& _ostr) const
{
  const PropertyContainer::Properties& vps = vprops_.properties();
  const PropertyContainer::Properties& hps = hprops_.properties();
  const PropertyContainer::Properties& eps = eprops_.properties();
  const PropertyContainer::Properties& fps = fprops_.properties();
  const PropertyContainer::Properties& mps = mprops_.properties();

  PropertyContainer::Properties::const_iterator it;

  _ostr << vprops_.size() << " vprops:\n";
  for (it=vps.begin(); it!=vps.end(); ++it)
  {
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);
  }
  _ostr << hprops_.size() << " hprops:\n";
  for (it=hps.begin(); it!=hps.end(); ++it)
  {
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);
  }
  _ostr << eprops_.size() << " eprops:\n";
  for (it=eps.begin(); it!=eps.end(); ++it)
  {
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);
  }
  _ostr << fprops_.size() << " fprops:\n";
  for (it=fps.begin(); it!=fps.end(); ++it)
  {
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);
  }
  _ostr << mprops_.size() << " mprops:\n";
  for (it=mps.begin(); it!=mps.end(); ++it)
  {
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);
  }
}



void BaseKernel::vprop_stats( std::string& _string ) const
{
  _string.clear();

  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& vps = vprops_.properties();
  for (it=vps.begin(); it!=vps.end(); ++it)
    *it == NULL ? _string += "[deleted] \n" : _string += (*it)->name();

}

void BaseKernel::hprop_stats( std::string& _string ) const
{
  _string.clear();

  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& hps = hprops_.properties();
  for (it=hps.begin(); it!=hps.end(); ++it)
    *it == NULL ? _string += "[deleted] \n" : _string += (*it)->name();

}

void BaseKernel::eprop_stats( std::string& _string ) const
{
  _string.clear();

  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& eps = eprops_.properties();
  for (it=eps.begin(); it!=eps.end(); ++it)
    *it == NULL ? _string += "[deleted] \n" : _string += (*it)->name();

}
void BaseKernel::fprop_stats( std::string& _string ) const
{
  _string.clear();

  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& fps = fprops_.properties();
  for (it=fps.begin(); it!=fps.end(); ++it)
    *it == NULL ? _string += "[deleted] \n" : _string += (*it)->name();

}

void BaseKernel::mprop_stats( std::string& _string ) const
{
  _string.clear();

  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& mps = mprops_.properties();
  for (it=mps.begin(); it!=mps.end(); ++it)
    *it == NULL ? _string += "[deleted] \n" : _string += (*it)->name();

}

void BaseKernel::vprop_stats(std::ostream& _ostr ) const
{
  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& vps = vprops_.properties();
  for (it=vps.begin(); it!=vps.end(); ++it)
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);

}

void BaseKernel::hprop_stats(std::ostream& _ostr ) const
{
  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& hps = hprops_.properties();
  for (it=hps.begin(); it!=hps.end(); ++it)
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);

}
void BaseKernel::eprop_stats(std::ostream& _ostr ) const
{
  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& eps = eprops_.properties();
  for (it=eps.begin(); it!=eps.end(); ++it)
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);

}
void BaseKernel::fprop_stats(std::ostream& _ostr ) const
{
  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& fps = fprops_.properties();
  for (it=fps.begin(); it!=fps.end(); ++it)
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);

}
void BaseKernel::mprop_stats(std::ostream& _ostr ) const
{
  PropertyContainer::Properties::const_iterator it;
  const PropertyContainer::Properties& mps = mprops_.properties();
  for (it=mps.begin(); it!=mps.end(); ++it)
    *it == NULL ? (void)(_ostr << "[deleted]" << "\n") : (*it)->stats(_ostr);

}


}
