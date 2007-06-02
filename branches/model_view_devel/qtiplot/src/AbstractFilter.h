/***************************************************************************
    File                 : AbstractFilter.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Knut Franke
    Email (use @ for *)  : knut.franke*gmx.de
    Description          : Base class for all analysis operations.

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef ABSTRACT_FILTER_H
#define ABSTRACT_FILTER_H

#include "AbstractDataSource.h"
#include <QVector>

// forward declaration, class follows
class AbstractFilter;
/**
 * \brief Catches signals and redirects them to AbstractFilter.
 *
 * This class makes it possible for AbstractFilter to receive signals without being a QObject.
 * This way, it can provide standard reactions to changes in input data and still allow filter
 * classes to inherit from QObject. See TruncatedDoubleDataSource for a typical example that
 * would get considerably more complicated if it couldn't inherit from both AbstractFilter
 * and AbstractDoubleDataSource.
 */
class AbstractFilterSlotMachine : public QObject {
	Q_OBJECT
	public:
		AbstractFilterSlotMachine(AbstractFilter *parent) : d_parent(parent) {}
		public slots:
			void inputDescriptionAboutToChange(AbstractDataSource* source);
			void inputDescriptionChanged(AbstractDataSource* source);
			void inputPlotDesignationAboutToChange(AbstractDataSource* source);
			void inputPlotDesignationChanged(AbstractDataSource* source);
			void inputDataAboutToChange(AbstractDataSource* source);
			void inputDataChanged(AbstractDataSource* source);
			void inputAboutToBeReplaced(AbstractDataSource* source, AbstractDataSource* replacement);
	private:
		AbstractFilter *d_parent;
};

/**
 * \brief Base class for all analysis operations.
 *
 * AbstractFilter provides an abstraction for analysis operations. It is modelled on an
 * electronic filtering circuit: From the outside, a filter appears as a black box with
 * a number of input and output ports (obviously, those numbers do not necessarily agree).
 * 
 * You can connect one AbstractDataSource to each input port using
 * input(int port, AbstractDataSource *source). Every output(int port) is realized
 * again by an AbstractDataSource, which you can connect to as many other filters, tables
 * or plots as you like.
 * Furthermore, you can use numInputs() and numOutputs() to query the number of
 * input and output ports, respectively. This allows generic filter handling routines
 * to be written, which is important for using filters provided by plugins.
 *
 * The main design goal was to make implementing new filters as easy as possible.
 * Only the pure virtual methods numInputs(), numOutputs() and output() have to be provided
 * for the simples possible implementation (CopyThroughFilter). To the same end, a little
 * additional complexity has been accepted in the form of AbstractFilterSlotMachine,
 * which on the other hand greatly simplifies filters with only one output port like
 * TruncatedDoubleDataSource (DifferentiatonFilter uses a similar approach, although
 * it provides two output ports).
 *
 * While filters can optionally handle inputs themselves by reimplementing input(),
 * possibly rejecting inputs based on their data type, for most cases the default
 * implementation will be sufficient: the input port is checked for validity against numInputs()
 * and, having passed, the AbstractDataSource is recorded in #d_inputs.
 * Signals are automatically connected to a set of corresponding virtual methods, which
 * can be used by implementations to react on changes in their inputs.
 *
 * Its simplicity of use notwithstanding, AbstractFilter provides a powerful and versatile
 * basis also for analysis operations that would not commonly be referred to as "filter".
 * An example of such a more advanced filter implementation is StatisticsFilter.
 */
class AbstractFilter
{
	public:
		//! Standard constructor.
		AbstractFilter() : d_slot_machine(this) {}
		//! Destructor.
		virtual ~AbstractFilter() {}

		//! Return the number of input port supported by the filter or -1 if any number of inputs is acceptable.
		virtual int numInputs() const = 0;
		/**
		 * \brief Return the number of output ports provided by the filter.
		 *
		 * Note that this number need not be static, but can be dynamically determined, for example
		 * based on the inputs provided to the filter.
		 */
		virtual int numOutputs() const = 0;
		/**
		 * \brief Connect the provided data source to the specified input port.
		 *
		 * \returns true if the connection was accepted, false otherwise.
		 */
		virtual bool input(int port, AbstractDataSource *source);
		/**
		 * \brief Get the data source associated with the specified output port.
		 *
		 * The returned pointer may be 0, for example if not all required input ports
		 * have been connected.
		 */
		virtual AbstractDataSource* output(int port) const = 0;
		// virtual void saveTo(QXmlStreamWriter *) = 0;
		// virtual void loadFrom(QXmlStreamReader *) = 0;

	protected:
		virtual void inputDescriptionAboutToChange(AbstractDataSource* source);
		virtual void inputDescriptionAboutToChange(int port) { Q_UNUSED(port); }
		virtual void inputDescriptionChanged(AbstractDataSource* source);
		virtual void inputDescriptionChanged(int port) { Q_UNUSED(port); }
		virtual void inputPlotDesignationAboutToChange(AbstractDataSource* source);
		virtual void inputPlotDesignationAboutToChange(int port) { Q_UNUSED(port); }
		virtual void inputPlotDesignationChanged(AbstractDataSource* source);
		virtual void inputPlotDesignationChanged(int port) { Q_UNUSED(port); }
		virtual void inputDataAboutToChange(AbstractDataSource* source);
		virtual void inputDataAboutToChange(int port) { Q_UNUSED(port); }
		virtual void inputDataChanged(AbstractDataSource* source);
		virtual void inputDataChanged(int port) { Q_UNUSED(port); }
		virtual void inputAboutToBeReplaced(AbstractDataSource* source, AbstractDataSource* replacement);
		QVector<AbstractDataSource*> d_inputs;

	private:
		friend class AbstractFilterSlotMachine;
		AbstractFilterSlotMachine d_slot_machine;
};

/**
 * \brief Filter which copies all provided inputs unaltered to an equal number of outputs.
 *
 * This is probably the simplest filter you can possibly write.
 * It accepts an arbitrary number of inputs and provides the same AbstractDataSource objects
 * as outputs again.
 */
class CopyThroughFilter : public AbstractFilter
{
	//! Accept any number of inputs.
	virtual int numInputs() const { return -1; }
	//! Provide as many output port as inputs have been connected.
	virtual int numOutputs() const { return d_inputs.size(); }
	//! When asked for an output port, just return the corresponding input port.
	virtual AbstractDataSource* output(int port) const { return d_inputs.value(port); }
};

#endif // ifndef ABSTRACT_FILTER_H
