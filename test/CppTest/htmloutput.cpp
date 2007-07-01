// ---
//
// $Id: htmloutput.cpp,v 1.2 2004/02/04 21:08:23 nilu Exp $
//
// CppTest - A C++ Unit Testing Framework
// Copyright (c) 2003 Niklas Lundell
//
// ---
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// ---

#include <algorithm>
#include <sstream>

#if (defined(__WIN32__) || defined(WIN32))
# include "winconfig.h"
#else
# include "config.h"
#endif
#include "cpptest-htmloutput.h"
#include "utils.h"

using namespace std;

namespace Test
{
	namespace
	{
		void
		header(ostream& os, string name)
		{
			if (!name.empty())
				name += " ";
			
			os <<
				"<!doctype doctype public \"-//W3C//DTD HTML 4.0 Transitional//EN\">\n"
				"<html>\n"
				"<head>\n"
				"  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n"
				"  <meta name=\"generator\" content=\"CppTest - http://cpptest.sourceforge.net\">\n"
				"  \n"
				"  <title>" << name << "Unit Tests Results</title>\n"
				"  \n"
				"  <style type=\"text/css\" media=\"screen\">\n"
				"    <!--\n"
				"    HR  {\n"
				"      width: 100%;\n"
				"      border-width: 0px;\n"
				"      height: 1px;\n"
				"      color: #cccccc;\n"
				"      background-color: #cccccc;\n"
				"      padding: 0px;\n"
				"    }\n"
				"    \n"
				"    .table_title {\n"
				"      background-color: #a5cef7;\n"
				"      font-weight: bold;\n"
				"    }\n"
				"    \n"
				"    .table_success {\n"
				"      background-color: #efefe7;\n"
				"    }\n"
				"    \n"
				"    .table_error {\n"
				"      color: #ff0808;\n"
				"      background-color: #efefe7;\n"
				"      font-weight: bold;\n"
				"    }\n"
				"    -->\n"
				"  </style>\n"
				"</head>\n"
				"\n"
				"<body>\n"
				"\n"
				"<h1><a name=\"top\"></a>" << name << "Unit Tests Results</h1>\n"
				"\n"
				"<div align=\"right\">\n"
				"Designed by <a href=\"http://cpptest.sourceforge.net\">CppTest</a>\n"
				"</div>\n"
				"<hr>\n"
				"\n";
		}
		
		void
		footer(ostream& os)
		{
			os << "\n</body>\n</html>\n";
		}
		
		void
		marker(ostream& os, const string& mark)
		{
			os << "<a name=\"" << mark << "\">";
		}
		
		void
		back_ref(ostream& os, const string& ref, bool prepend_newline = true)
		{
			if (prepend_newline)
				os << "<br>\n";
			os	<< "<a href=\"#" << ref
				<< "\">Back to " << ref << "</a>\n<br><br>\n";
		}
		
		void
		sub_title(ostream& os, const string& title, int size)
		{
			ostringstream h;
			h << "h" << size;
			os << "<" << h.str() << ">" << title << "</" << h.str() << ">\n";
		}
			
		void
		table_header(ostream& os)
		{
			os << "<table border=\"0\" width=\"100%\">\n";
		}
		
		void
		table_footer(ostream& os)
		{
			os << "</table>\n";
		}
		
		void
		table_tr_header(ostream& os)
		{
			os << "  <tr>\n";
		}
		
		void
		table_tr_footer(ostream& os)
		{
			os << "  </tr>\n";
		}
		
		enum ClassType { Title, Success, Error };
		
		void
		table_entry(ostream& os, ClassType type, const string& s,
					int width = 0, const string& link = "")
		{
			static const char* class_types[] = { "title", "success", "error" };
			
			os << "    <td class=\"table_" << class_types[type] << "\"";
			if (width)
				os << "width=\"" << width << "%\"";
			os << ">";
			if (!link.empty())
				os << "<a href=\"#" << link << "\">" << s << "</a>";
			else
				os << s;
			os << "</td>\n";	
		}
						
	} // anonymous namespace
	
	// Test suite table
	//
	struct HtmlOutput::SuiteRow
	{
		ostream& _os;
		SuiteRow(ostream& os) : _os(os) {}
		void operator()(const SuiteInfo& si)
		{
			ClassType 		type(si._errors > 0 ? Error : Success);
			ostringstream 	ss;

			table_tr_header(_os);
			  table_entry(_os, type, si._name, 0, si._name);
			  ss.str(""), ss << si._tests.size();
			  table_entry(_os, type, ss.str(), 10);
			  ss.str(""), ss << si._errors;
			  table_entry(_os, type, ss.str(), 10);
			  ss.str(""),  ss << correct(si._tests.size(), si._errors) << "%";
			  table_entry(_os, type, ss.str(), 10);
			  ss.str(""), ss << si._time;
			  table_entry(_os, type, ss.str(), 10);
			table_tr_footer(_os);
		}
	};
	
	// Individual tests tables, tests
	//
	struct HtmlOutput::TestRow
	{
		bool		_incl_ok_tests;
		ostream& 	_os;
		TestRow(ostream& os, bool incl_ok_tests)
			: _incl_ok_tests(incl_ok_tests), _os(os) {}
		void operator()(const TestInfo& ti)
		{
			if (!ti._success || _incl_ok_tests)
			{
				string			link = ti._success ? string(""):
									   ti._sources.front().suite() + "_" + ti._name;
				ClassType 		type(ti._success ? Success : Error);
				ostringstream 	ss;
				
				table_tr_header(_os);
				  table_entry(_os, type, ti._name, 0, link);
				  ss.str(""),  ss << ti._sources.size();
				  table_entry(_os, type, ss.str());
				  table_entry(_os, type, ti._success ? "true" : "false");
				  ss.str(""), ss << ti._time;
				  table_entry(_os, type, ss.str());
				table_tr_footer(_os);
			}
		}
	};
	
	// Individual tests tables, header
	//
	struct HtmlOutput::TestSuiteRow
	{
		bool		_incl_ok_tests;
		ostream& 	_os;
		TestSuiteRow(ostream& os, bool incl_ok_tests) 
			: _incl_ok_tests(incl_ok_tests), _os(os) {}
		void operator()(const SuiteInfo& si)
		{
			ostringstream ss;
			
			marker(_os, si._name);
			sub_title(_os, "Suite: " + si._name, 3);
			table_header(_os);
			  table_tr_header(_os);
			    table_entry(_os, Title, "Name");
			    table_entry(_os, Title, "Errors", 10);
			    table_entry(_os, Title, "Success", 10);
			    table_entry(_os, Title, "Time (s)", 10);
			  table_tr_footer(_os);
			  for_each(si._tests.begin(), si._tests.end(), 
					   TestRow(_os, _incl_ok_tests));
			table_footer(_os);
			back_ref(_os, "top");
		}
	};
	
	// Individual tests result tables
	//
	struct HtmlOutput::TestResult
	{
		ostream& _os;
		TestResult(ostream& os) : _os(os) {}
		void operator()(const Source& s)
		{
			const int TitleSize = 15;
			
			ostringstream ss;
			
			table_header(_os);
			  table_tr_header(_os);
				table_entry(_os, Title, "Test", TitleSize);
				table_entry(_os, Success, s.suite() + "::" + s.test());
			  table_tr_footer(_os);
			  table_tr_header(_os);
				table_entry(_os, Title, "File", TitleSize);
				ss << s.file() << ":" << s.line();
				table_entry(_os, Success, ss.str());
			  table_tr_footer(_os);
			  table_tr_header(_os);
				table_entry(_os, Title, "Message", TitleSize);
				table_entry(_os, Success, s.message());
			  table_tr_footer(_os);
			table_footer(_os);
			_os << "<br>\n";
		}
	};
	
	// All tests result tables
	//
	struct HtmlOutput::TestResultAll
	{
		ostream& _os;
		TestResultAll(ostream& os) : _os(os) {}
		void operator()(const TestInfo& ti)
		{
			if (!ti._success)
			{
				const string& suite = ti._sources.front().suite();
				
				marker(_os, suite + "_" + ti._name);
				for_each(ti._sources.begin(), ti._sources.end(), TestResult(_os));
				back_ref(_os, suite, false);
			}
		}
	};
	
	// Individual tests result tables, iterator
	//
	struct HtmlOutput::SuiteTestResult
	{
		ostream& _os;
		SuiteTestResult(ostream& os) : _os(os) {}
		void operator()(const SuiteInfo& si)
		{
			for_each(si._tests.begin(), si._tests.end(), TestResultAll(_os));
		}
	};
	
	/// Generates the HTML table. This function should only be called after
	/// run(), when all tests have been executed.
	///
	/// \param os            Output stream.
	/// \param incl_ok_tests Set if successful tests should be shown; 
	///                      false otherwise.
	/// \param name          Name of generated report.
	///
    void
	HtmlOutput::generate(ostream& os, bool incl_ok_tests, const string& name)
	{
		ClassType 		type(_total_errors > 0 ? Error : Success);
		ostringstream 	ss;
		
		header(os, name);
		
		// Table: Summary
		//
		sub_title(os, "Summary", 2);
		table_header(os);
		  table_tr_header(os);
		    table_entry(os, Title, "Tests", 30);
		    table_entry(os, Title, "Errors", 30);
		    table_entry(os, Title, "Success", 30);
		    table_entry(os, Title, "Time (s)", 10);
		  table_tr_footer(os);
		  table_tr_header(os);
		    ss.str(""), ss << _total_tests;
 		    table_entry(os, type, ss.str(), 30);
			ss.str(""),  ss << _total_errors;
		    table_entry(os, type, ss.str(), 30);
			ss.str(""),  ss << correct(_total_tests, _total_errors) << "%";
		    table_entry(os, type, ss.str(), 30);
			ss.str(""), ss << _total_time;
		    table_entry(os, type, ss.str(), 10);
		  table_tr_footer(os);
		table_footer(os);
		os << "<hr>\n\n";
		
		// Table: Test suites
		//
		sub_title(os, "Test suites", 2);
		table_header(os);
		  table_tr_header(os);
		    table_entry(os, Title, "Name");
		    table_entry(os, Title, "Tests",   10);
		    table_entry(os, Title, "Errors",  10);
		    table_entry(os, Title, "Success",    10);
		    table_entry(os, Title, "Time (s)", 10);
		  table_tr_footer(os);
		  for_each(_suites.begin(), _suites.end(), SuiteRow(os));
		table_footer(os);
		os << "<hr>\n\n";
		
		// Individual tests tables
		//
		for_each(_suites.begin(), _suites.end(), TestSuiteRow(os, incl_ok_tests));
		os << "<hr>\n\n";
		
		// Individual tests result tables
		//
		sub_title(os, "Test results", 2);
		for_each(_suites.begin(), _suites.end(), SuiteTestResult(os));
		os << "<hr>\n\n";
		
		// EOF
		//
		footer(os);
	}
	
} // namespace Test

