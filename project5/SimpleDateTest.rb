require 'minitest/autorun'
require './SimpleDate.rb'

class SimpleDateTest < Minitest::Test

	#
	# Unit tests for initialiaze() method
	# 
	def test_initialize_Valid_T1()
		d = SimpleDate.new(1, 1, 2010)
		assert_equal(1, d.month())
		assert_equal(1, d.day())
		assert_equal(2010, d.year())
	end	
	
	def test_initialize_Valid_T2()
		d = SimpleDate.new(1, 31, 2010)
		assert_equal(1, d.month())
		assert_equal(31, d.day())
		assert_equal(2010, d.year())
	end	
	
	def test_initialize_Valid_T3()
		d = SimpleDate.new(2, 28, 1900)
		assert_equal(2, d.month())
		assert_equal(28, d.day())
		assert_equal(1900, d.year())
	end
	
	def test_initialize_Valid_T4()
		d = SimpleDate.new(2, 28, 2009)
		assert_equal(2, d.month())
		assert_equal(28, d.day())
		assert_equal(2009, d.year())
	end
	
	def test_initialize_Valid_T5()
		d = SimpleDate.new(2, 28, 2012)
		assert_equal(2, d.month())
		assert_equal(28, d.day())
		assert_equal(2012, d.year())
	end
	
	def test_initialize_Valid_T6()
		d = SimpleDate.new(2, 29, 2012)
		assert_equal(2, d.month())
		assert_equal(29, d.day())
		assert_equal(2012, d.year())
	end
	
	def test_initialize_Valid_T7()
		d = SimpleDate.new(3, 31, 2000)
		assert_equal(3, d.month())
		assert_equal(31, d.day())
		assert_equal(2000, d.year())
	end
	
	def test_initialize_Valid_T8()
		d = SimpleDate.new(4, 30, 2000)
		assert_equal(4, d.month())
		assert_equal(30, d.day())
		assert_equal(2000, d.year())
	end
	
	def test_initialize_Valid_T9()
		d = SimpleDate.new(5, 31, 2010)
		assert_equal(5, d.month())
		assert_equal(31, d.day())
		assert_equal(2010, d.year())
	end
	
	def test_initialize_Valid_T10() 
		d = SimpleDate.new(6, 30, 2006)
		assert_equal(6, d.month())
		assert_equal(30, d.day())
		assert_equal(2006, d.year())
	end
	
	def test_initialize_Valid_T11() 
		d = SimpleDate.new(7, 31, 2007)
		assert_equal(7, d.month())
		assert_equal(31, d.day())
		assert_equal(2007, d.year())
	end
	
	def test_initialize_Valid_T12() 
		d = SimpleDate.new(8, 31, 2008)
		assert_equal(8, d.month())
		assert_equal(31, d.day())
		assert_equal(2008, d.year())
	end
	
	def test_initialize_Valid_T13() 
		d = SimpleDate.new(9, 30, 2002)
		assert_equal(9, d.month())
		assert_equal(30, d.day())
		assert_equal(2002, d.year())
	end
	
	def test_initialize_Valid_T14() 
		d = SimpleDate.new(10, 31, 1997)
		assert_equal(10, d.month())
		assert_equal(31, d.day())
		assert_equal(1997, d.year())
	end
	
	def test_initialize_Valid_T15() 
		d = SimpleDate.new(11, 30, 1992)
		assert_equal(11, d.month())
		assert_equal(30, d.day())
		assert_equal(1992, d.year())
	end
	
	def test_initialize_Valid_T16() 
		d = SimpleDate.new(12, 31, 2034)
		assert_equal(12, d.month())
		assert_equal(31, d.day())
		assert_equal(2034, d.year())
	end
	
	def test_initialize_Invalid_T17() 
		assert_raises(ArgumentError)  { SimpleDate.new(-1, 1, 2010) }
	end	
	
	def test_initialize_Invalid_T18() 
		assert_raises(ArgumentError)  { SimpleDate.new(0, 1, 2010) }
	end
	
	def test_initialize_Invalid_T19() 
		assert_raises(ArgumentError)  { SimpleDate.new(1, 0, 2010) }
	end
	
	def test_initialize_Invalid_T20()
		assert_raises(ArgumentError)  { SimpleDate.new(1, -3, 2010) }
	end
	
	def test_initialize_Invalid_T21()
		assert_raises(ArgumentError)  { SimpleDate.new(1, 1, 1751) }
	end
	
	def test_initialize_Invalid_T22()
		assert_raises(ArgumentError)  { SimpleDate.new(1, 7, -2023) }
	end
	
	def test_initialize_Invalid_T23()
		assert_raises(ArgumentError)  { SimpleDate.new(1, 32, 2010) }
	end
	
	def test_initialize_Invalid_T24()
		assert_raises(ArgumentError)  { SimpleDate.new(2, 29, 2009) }
	end
	
	def test_initialize_Invalid_T25()
		assert_raises(ArgumentError)  { SimpleDate.new(2, 29, 1900) }
	end
	
	def test_initialize_Invalid_T26()
		assert_raises(ArgumentError)  { SimpleDate.new(2, 30, 2009) }
	end
	
	def test_initialize_Invalid_T27()
		assert_raises(ArgumentError)  { SimpleDate.new(2, 30, 2012) }
	end
	
	def test_initialize_Invalid_T28()
		assert_raises(ArgumentError)  { SimpleDate.new(3, 32, 2000) }
	end
	
	def test_initialize_Invalid_T29()
		assert_raises(ArgumentError)  { SimpleDate.new(4, 31, 2000) }
	end
	
	def test_initialize_Invalid_T30()
		assert_raises(ArgumentError)  { SimpleDate.new(5, 32, 2000) }
	end
	
	def test_initialize_Invalid_T31()
		assert_raises(ArgumentError)  { SimpleDate.new(6, 31, 2000) }
	end
	
	def test_initialize_Invalid_T32()
		assert_raises(ArgumentError)  { SimpleDate.new(7, 32, 2000) }
	end
	
	def test_initialize_Invalid_T33()
		assert_raises(ArgumentError)  { SimpleDate.new(8, 32, 2000) }
	end
	
	def test_initialize_Invalid_T34()
		assert_raises(ArgumentError)  { SimpleDate.new(9, 31, 2000) }
	end
	
	def test_initialize_Invalid_T35()
		assert_raises(ArgumentError)  { SimpleDate.new(10, 32, 2000) }
	end
	
	def test_initialize_Invalid_T36()
		assert_raises(ArgumentError)  { SimpleDate.new(11, 31, 2000) }
	end
	
	def test_initialize_Invalid_T37()
		assert_raises(ArgumentError)  { SimpleDate.new(12, 32, 2000) }
	end
	
  def test_initialize_Invalid_T38()
     assert_raises(ArgumentError)  { SimpleDate.new("doe", 32, 2000) }
  end
  
  def test_initialize_Invalid_T39()
    assert_raises(ArgumentError)  { SimpleDate.new(12, 19.95, 2000) }
  end
  
  def test_initialize_Invalid_T40()
    assert_raises(ArgumentError)  { SimpleDate.new(12, 32, true) }
  end
   
  def test_initialize_Invalid_T41()
    assert_raises(ArgumentError)  { SimpleDate.new("doe", 32.50, false) }
  end   
  
  def test_initialize_Invalid_T42()
    assert_raises(ArgumentError)  { SimpleDate.new([1,2], '32.50', nil) }
  end
      
	#
	# Unit tests for <=> method
	#
	def test_compareTo_T1()
		d = SimpleDate.new(11, 30, 1992)
		assert((d <=> d) == 0)
	end
	
	def test_compareTo_T2()
		d1 = SimpleDate.new(2, 29, 2012)
		d2 = SimpleDate.new(2, 29, 2012)
		assert((d1 <=> d2) == 0)
	end	
	
	def test_compareTo_T3()
		d1 = SimpleDate.new(3, 10, 2011)
		d2 = SimpleDate.new(6, 10, 2011)
		assert((d1 <=> d2) < 0)
	end
	
	def test_compareTo_T4()
		d1 = SimpleDate.new(3, 10, 2011)
		d2 = SimpleDate.new(6, 10, 2012)
		assert((d1 <=> d2) < 0)
	end
	
	def test_compareTo_T5()
		d1 = SimpleDate.new(3, 10, 2011)
		d2 = SimpleDate.new(6, 10, 2011)
		assert((d2 <=> d1) > 0)	
	end
	
	def test_compareTo_T6()
		d1 = SimpleDate.new(3, 10, 2011)
		d2 = SimpleDate.new(6, 10, 2012)
		assert((d2 <=> d1) > 0)
	end	

	def test_compareTo_T7()
		d1 = SimpleDate.new(3, 10, 2011)
		assert_nil(d1 <=> 5)
	end	

	#
	# Unit tests for dayOfWeek() method
	#
	def test_dayOfWeek_T1()
		assert_equal(SimpleDate::SATURDAY, SimpleDate.new(2,29,1908).dayOfWeek())
	end
	
	def test_dayOfWeek_T2()
		assert_equal(SimpleDate::MONDAY, SimpleDate.new(1,1,1753).dayOfWeek())
	end
	
	def test_dayOfWeek_T3()
		assert_equal(SimpleDate::TUESDAY, SimpleDate.new(1,2,1753).dayOfWeek())
	end
	
	def test_dayOfWeek_T4()
		assert_equal(SimpleDate::WEDNESDAY, SimpleDate.new(1,3,1753).dayOfWeek())
	end
	
	def test_dayOfWeek_T5()
		assert_equal(SimpleDate::THURSDAY, SimpleDate.new(1,4,1753).dayOfWeek())
	end
	
	def test_dayOfWeek_T6()
		assert_equal(SimpleDate::FRIDAY, SimpleDate.new(1,5,1753).dayOfWeek())
	end
	
	def test_dayOfWeek_T7()
		assert_equal(SimpleDate::SATURDAY, SimpleDate.new(1,6,1753).dayOfWeek())
	end
	
	def test_dayOfWeek_T8()
		assert_equal(SimpleDate::FRIDAY, SimpleDate.new(1,6,2012).dayOfWeek())
	end
	
	def test_dayOfWeek_T9()
		assert_equal(SimpleDate::WEDNESDAY, SimpleDate.new(2,1,2012).dayOfWeek())
	end
	
	def test_dayOfWeek_T10()
		assert_equal(SimpleDate::WEDNESDAY, SimpleDate.new(2,29,2012).dayOfWeek())
	end
	
	def test_dayOfWeek_T11()
		assert_equal(SimpleDate::MONDAY, SimpleDate.new(12,31,2012).dayOfWeek())
	end
	
	def test_dayOfWeek_T12()
		assert_equal(SimpleDate::TUESDAY, SimpleDate.new(1,1,2013).dayOfWeek())
	end
	
	#
	# Unit tests for daysInYear() class and instance methods
	#
	def test_daysInYear_T1()
		assert_equal(366, SimpleDate.daysInYear(1756))
	end	
	
	def test_daysInYear_T2()
		assert_equal(366, SimpleDate.daysInYear(1896))
	end
	
	def test_daysInYear_T3()
		assert_equal(366, SimpleDate.daysInYear(1904))
	end
	
	def test_daysInYear_T4()
		assert_equal(366, SimpleDate.daysInYear(1908))
	end
	
	def test_daysInYear_T5()
		assert_equal(365, SimpleDate.daysInYear(1897))
	end	
	
	def test_daysInYear_T6()
		assert_equal(365, SimpleDate.daysInYear(1898))
	end	
	
	def test_daysInYear_T7()
		assert_equal(365, SimpleDate.daysInYear(1899))
	end	
	
	def test_daysInYear_T8()
		assert_equal(365, SimpleDate.daysInYear(1900))
	end
	
	def test_daysInYear_instance_T1()
		assert_equal(366, SimpleDate.new(1,1,1756).daysInYear())
	end	
	
	def test_daysInYear_instance_T2()
		assert_equal(366, SimpleDate.new(1,1,1896).daysInYear())
	end	
	
	def test_daysInYear_instance_T3()
		assert_equal(366, SimpleDate.new(1,1,1904).daysInYear())
	end	
	
	def test_daysInYear_instance_T4()
		assert_equal(366, SimpleDate.new(1,1,1908).daysInYear())
	end
	
	def test_daysInYear_instance_T5()
		assert_equal(365, SimpleDate.new(1,1,1897).daysInYear())
	end	
	
	def test_daysInYear_instance_T6()
		assert_equal(365, SimpleDate.new(1,1,1898).daysInYear())
	end
	
	def test_daysInYear_instance_T7()
		assert_equal(365, SimpleDate.new(1,1,1899).daysInYear())
	end
	
	def test_daysInYear_instance_T8()
		assert_equal(365, SimpleDate.new(1,1,1900).daysInYear())
	end
	
	#
	# Units tests for leapYear?() class and instance methods
	#
	def test_leapYear_T1()
		assert(SimpleDate.leapYear?(1756))
	end
	
	def test_leapYear_T2()
		assert(SimpleDate.leapYear?(1896))
	end	
	
	def test_leapYear_T3()
		assert(SimpleDate.leapYear?(1904))
	end	

	def test_leapYear_T4()
		assert(SimpleDate.leapYear?(1908))
	end

	def test_leapYear_T5()
		assert(SimpleDate.leapYear?(1996))
	end

	def test_leapYear_T6()
		assert(SimpleDate.leapYear?(2000))
	end

	def test_leapYear_T7()
		assert(SimpleDate.leapYear?(2004))
	end

	def test_leapYear_T8()
		assert(SimpleDate.leapYear?(2008))
	end

	def test_leapYear_T9()
		assert(!SimpleDate.leapYear?(1753))
	end

	def test_leapYear_T10()
		assert(!SimpleDate.leapYear?(1897))
	end
	
	def test_leapYear_T11()
		assert(!SimpleDate.leapYear?(1898))
	end

	def test_leapYear_T12()
		assert(!SimpleDate.leapYear?(1899))
	end
	
	def test_leapYear_T13()
		assert(!SimpleDate.leapYear?(1900))
	end
	
	def test_leapYear_T14()
		assert(!SimpleDate.leapYear?(1901))
	end
	
	def test_leapYear_T15()
		assert(!SimpleDate.leapYear?(1902))
	end
	
	def test_leapYear_T16()
		assert(!SimpleDate.leapYear?(1903))
	end
	
	def test_leapYear_T17()
		assert(!SimpleDate.leapYear?(1999))
	end
	
	def test_leapYear_T18()
		assert(!SimpleDate.leapYear?(2001))
	end
	
	def test_leapYear_instance_T1()
		assert(SimpleDate.new(1,1,1756).leapYear?())
	end
	
	def test_leapYear_instance_T2()
		assert(SimpleDate.new(1,1,1896).leapYear?())
	end
	
	def test_leapYear_instance_T3()
		assert(SimpleDate.new(1,1,1904).leapYear?())
	end

	def test_leapYear_instance_T4()
		assert(SimpleDate.new(1,1,1908).leapYear?())
	end

	def test_leapYear_instance_T5()
		assert(SimpleDate.new(1,1,1996).leapYear?())
	end

	def test_leapYear_instance_T6()
		assert(SimpleDate.new(1,1,2000).leapYear?())
	end

	def test_leapYear_instance_T7()
		assert(SimpleDate.new(1,1,2004).leapYear?())
	end

	def test_leapYear_instance_T8()
		assert(SimpleDate.new(1,1,2008).leapYear?())
	end

	def test_leapYear_instance_T9()
		assert(!SimpleDate.new(1,1,1753).leapYear?())
	end

	def test_leapYear_instance_T10()
		assert(!SimpleDate.new(1,1,1897).leapYear?())
	end
	
	def test_leapYear_instance_T11()
		assert(!SimpleDate.new(1,1,1898).leapYear?())
	end

	def test_leapYear_instance_T12()
		assert(!SimpleDate.new(1,1,1899).leapYear?())
	end
	
	def test_leapYear_instance_T13()
		assert(!SimpleDate.new(1,1,1900).leapYear?())
	end
	
	def test_leapYear_instance_T14()
		assert(!SimpleDate.new(1,1,1901).leapYear?())
	end
	
	def test_leapYear_instance_T15()
		assert(!SimpleDate.new(1,1,1902).leapYear?())
	end
	
	def test_leapYear_instance_T16()
		assert(!SimpleDate.new(1,1,1903).leapYear?())
	end
	
	def test_leapYear_instance_T17()
		assert(!SimpleDate.new(1,1,1999).leapYear?())
	end
	
	def test_leapYear_instance_T18()
		assert(!SimpleDate.new(1,1,2001).leapYear?())
	end
	
	#
	# Unit tests for ordinalDate() method
	#
	def test_ordinalDate_T1()
		d = SimpleDate.new(1,1,2007)
		assert_equal(1, d.ordinalDate())
	end
	
	def test_ordinalDate_T2()
		d = SimpleDate.new(1,2,2007)
		assert_equal(2, d.ordinalDate())
	end
	
	def test_ordinalDate_T3()
		d = SimpleDate.new(1,31,2007)
		assert_equal(31, d.ordinalDate())
	end
	
	def test_ordinalDate_T4()
		d = SimpleDate.new(2,1,2007)
		assert_equal(32, d.ordinalDate())
	end
	
	def test_ordinalDate_T5()
		d = SimpleDate.new(2,28,2007)
		assert_equal(59, d.ordinalDate())
	end
	
	def test_ordinalDate_T6()
		d = SimpleDate.new(3,1,2007)
		assert_equal(60, d.ordinalDate())
	end
	
	def test_ordinalDate_T7()
		d = SimpleDate.new(3,31,2007)
		assert_equal(90, d.ordinalDate())
	end
	
	def test_ordinalDate_T8()
		d = SimpleDate.new(4,15,2007)
		assert_equal(105, d.ordinalDate())
	end
	
	def test_ordinalDate_T9()
		d = SimpleDate.new(5,15,2007)
		assert_equal(135, d.ordinalDate())
	end
	
	def test_ordinalDate_T10()
		d = SimpleDate.new(6,15,2007)
		assert_equal(166, d.ordinalDate())
	end
	
	def test_ordinalDate_T11()
		d = SimpleDate.new(7,15,2007)
		assert_equal(196, d.ordinalDate())
	end
	
	def test_ordinalDate_T12()
		d = SimpleDate.new(8,15,2007)
		assert_equal(227, d.ordinalDate())
	end
	
	def test_ordinalDate_T13()
		d = SimpleDate.new(9,15,2007)
		assert_equal(258, d.ordinalDate())
	end
	
	def test_ordinalDate_T14()
		d = SimpleDate.new(10,15,2007)
		assert_equal(288, d.ordinalDate())
	end
	
	def test_ordinalDate_T15()
		d = SimpleDate.new(11,15,2007)
		assert_equal(319, d.ordinalDate())
	end
	
	def test_ordinalDate_T16()
		d = SimpleDate.new(12,1,2007)
		assert_equal(335, d.ordinalDate())
	end
	
	def test_ordinalDate_T17()
		d = SimpleDate.new(12,31,2007)
		assert_equal(365, d.ordinalDate())
	end
	
	def test_ordinalDate_T18()
		d = SimpleDate.new(1,1,2008)
		assert_equal(1, d.ordinalDate())
	end
	
	def test_ordinalDate_T19()
		d = SimpleDate.new(1,2,2008)
		assert_equal(2, d.ordinalDate())
	end
	
	def test_ordinalDate_T20()
		d = SimpleDate.new(1,31,2008)
		assert_equal(31, d.ordinalDate())
	end
	
	def test_ordinalDate_T21()
		d = SimpleDate.new(2,1,2008)
		assert_equal(32, d.ordinalDate())
	end
	
	def test_ordinalDate_T22()
		d = SimpleDate.new(2,28,2008)
		assert_equal(59, d.ordinalDate())
	end
	
	def test_ordinalDate_T23()
		d = SimpleDate.new(3,1,2008)
		assert_equal(61, d.ordinalDate())
	end
	
	def test_ordinalDate_T24()
		d = SimpleDate.new(3,31,2008)
		assert_equal(91, d.ordinalDate())
	end
	
	def test_ordinalDate_T25()
		d = SimpleDate.new(4,15,2008)
		assert_equal(106, d.ordinalDate())
	end
	
	def test_ordinalDate_T26()
		d = SimpleDate.new(5,15,2008)
		assert_equal(136, d.ordinalDate())
	end
	
	def test_ordinalDate_T27()
		d = SimpleDate.new(6,15,2008)
		assert_equal(167, d.ordinalDate())
	end
	
	def test_ordinalDate_T28()
		d = SimpleDate.new(7,15,2008)
		assert_equal(197, d.ordinalDate())
	end
	
	def test_ordinalDate_T29()
		d = SimpleDate.new(8,15,2008)
		assert_equal(228, d.ordinalDate())
	end
	
	def test_ordinalDate_T30()
		d = SimpleDate.new(9,15,2008)
		assert_equal(259, d.ordinalDate())
	end
	
	def test_ordinalDate_T31()
		d = SimpleDate.new(10,15,2008)
		assert_equal(289, d.ordinalDate())
	end
	
	def test_ordinalDate_T32()
		d = SimpleDate.new(11,15,2008)
		assert_equal(320, d.ordinalDate())
	end
	
	def test_ordinalDate_T33()
		d = SimpleDate.new(12,1,2008)
		assert_equal(336, d.ordinalDate())
	end
	
	def test_ordinalDate_T34()
		d = SimpleDate.new(1,1,1753)
		assert_equal(1, d.ordinalDate())
	end
	
	#
	# Unit tests for nextDate() method
	#
  def test_nextDate_T1()
    assert_equal(SimpleDate.new(1,2,2012), SimpleDate.new(1,1,2012).nextDate())
  end
  	
  def test_nextDate_T2()
    assert_equal(SimpleDate.new(1,31,2012), SimpleDate.new(1,30,2012).nextDate())
  end	

  def test_nextDate_T3()
    assert_equal(SimpleDate.new(2,29,2012), SimpleDate.new(2,28,2012).nextDate())
  end
  
  def test_nextDate_T4()
    assert_equal(SimpleDate.new(1,1,2013), SimpleDate.new(12,31,2012).nextDate())
  end
  
  def test_nextDate_T5()
    assert_equal(SimpleDate.new(5,1,2012), SimpleDate.new(4,30,2012).nextDate())
  end
  
  def test_nextDate_T6()
    assert_equal(SimpleDate.new(6,1,2012), SimpleDate.new(5,31,2012).nextDate())
  end
  
  def test_nextDate_T7()
    assert_equal(SimpleDate.new(3,1,2011), SimpleDate.new(2,28,2011).nextDate())
  end  
    
	#
	# Unit tests for prevDate() method
	#
  def test_prevDate_T1()
    assert_equal(SimpleDate.new(12,31,2011), SimpleDate.new(1,1,2012).prevDate())
  end
  
  def test_prevDate_T2()
    assert_equal(SimpleDate.new(1,31,2012), SimpleDate.new(2,1,2012).prevDate())
  end
  
  def test_prevDate_T3()
    assert_equal(SimpleDate.new(2,29,2012), SimpleDate.new(3,1,2012).prevDate())
  end
  
  def test_prevDate_T4()
    assert_equal(SimpleDate.new(2,28,2011), SimpleDate.new(3,1,2011).prevDate())
  end  
  	
  def test_prevDate_T5()
    assert_equal(SimpleDate.new(3,31,2012), SimpleDate.new(4,1,2012).prevDate())
  end
  
  def test_prevDate_T6()
    assert_equal(SimpleDate.new(4,30,2012), SimpleDate.new(5,1,2012).prevDate())
  end
  
  def test_prevDate_T7()
    assert_equal(SimpleDate.new(7,31,2012), SimpleDate.new(8,1,2012).prevDate())
  end
  
  #
  # Unit tests for daysAgo() method
  #
  def test_daysAgo_T1()
    assert_equal(SimpleDate.new(1,25,2011), SimpleDate.new(1,28,2011).daysAgo(3))
  end
  
  def test_daysAgo_T2()
    assert_equal(SimpleDate.new(1,28,2011), SimpleDate.new(1,25,2011).daysAgo(-3))    
  end
  
  def test_daysAgo_T3()
    assert_equal(SimpleDate.new(1,25,2011), SimpleDate.new(1,25,2011).daysAgo(0))    
  end
  
  def test_daysAgo_T4()
    assert_equal(SimpleDate.new(12,28,2010), SimpleDate.new(1,2,2011).daysAgo(5))    
  end
  
  def test_daysAgo_T5()
    assert_equal(SimpleDate.new(11,30,2011), SimpleDate.new(12,31,2011).daysAgo(31))    
  end
  
  def test_daysAgo_T6()
    assert_equal(SimpleDate.new(1,31,2012), SimpleDate.new(3,1,2012).daysAgo(30))   
  end
  
  def test_daysAgo_T7()
    assert_raises(ArgumentError) { SimpleDate.new(1,2,1753).daysAgo(3) }
  end
    
  #
  # Unit tests for daysFromNow() method
  #
  def test_daysFromNow_1()
    assert_equal(SimpleDate.new(1,28,2011), SimpleDate.new(1,25,2011).daysFromNow(3))
  end
  
  def test_daysFromNow_2()
    assert_equal(SimpleDate.new(1,25,2011), SimpleDate.new(1,28,2011).daysFromNow(-3))    
  end

  def test_daysFromNow_T3()
    assert_equal(SimpleDate.new(1,25,2011), SimpleDate.new(1,25,2011).daysFromNow(0))    
  end
  
  def test_daysFromNow_T4()
    assert_equal(SimpleDate.new(1,2,2011), SimpleDate.new(12,28,2010).daysFromNow(5))    
  end
  
  def test_daysFromNow_T5()
    assert_equal(SimpleDate.new(12,31,2011), SimpleDate.new(11,30,2011).daysFromNow(31))    
  end
  
  def test_daysFromNow_T6()
    assert_equal(SimpleDate.new(3,1,2012), SimpleDate.new(1,31,2012).daysFromNow(30))    
  end
  
  def test_daysFromNow_T7()
    assert_equal(SimpleDate.new(3,1,2012), SimpleDate.new(2,1,2012).daysFromNow(29))    
  end
  
  def test_daysFromNow_T8()
    assert_raises(ArgumentError) { SimpleDate.new(1,2,1753).daysFromNow(-3) }
  end
  
  def test_daysFromNow_T9()
    assert_equal(SimpleDate.new(1,1,2003), SimpleDate.new(1,1,2001).daysFromNow(730))    
  end
  
  def test_daysFromNow_T10()
    assert_equal(SimpleDate.new(1,1,2001), SimpleDate.new(1,1,2003).daysFromNow(-730))    
  end
  
  def test_daysFromNow_T11()
    assert_equal(SimpleDate.new(12,31,2002), SimpleDate.new(12,31,2000).daysFromNow(730))    
  end
  
  def test_daysFromNow_T12()
    assert_equal(SimpleDate.new(12,31,2000), SimpleDate.new(12,31,2002).daysFromNow(-730))    
  end
  
  def test_daysFromNow_T13()
    assert_equal(SimpleDate.new(1,1,2013), SimpleDate.new(1,1,2012).daysFromNow(366))    
  end
  
  #
  # Unit tests for daysInMonth() class method
  #
  def test_daysInMonth_T1()
    assert_equal(31, SimpleDate.daysInMonth(1,2012))
  end
  
  def test_daysInMonth_T2()
    assert_equal(29, SimpleDate.daysInMonth(2,2012))
  end
    
  def test_daysInMonth_T3()
    assert_equal(28, SimpleDate.daysInMonth(2,2011))
  end
  
  def test_daysInMonth_T4()
    assert_equal(31, SimpleDate.daysInMonth(12,2012))
  end
    
  def test_daysInMonth_T5()
    assert_equal(30, SimpleDate.daysInMonth(4,2012))
  end
    
  #
  # Unit tests for validDate?() class method
  #
  def test_validDate_T1() 
    assert(SimpleDate.validDate?(1, 1, 2010))
  end
    
  def test_validDate_T2() 
    assert(SimpleDate.validDate?(1, 31, 2010))
  end  
 
  def test_validDate_T3() 
    assert(SimpleDate.validDate?(2, 28, 1900))
  end 
  
  def test_validDate_T4() 
    assert(SimpleDate.validDate?(2, 28, 2009))
  end
 
  def test_validDate_T5() 
    assert(SimpleDate.validDate?(2, 28, 2012))
  end 

  def test_validDate_T6() 
    assert(SimpleDate.validDate?(2, 29, 2012))
  end 
  
  def test_validDate_T7() 
    assert(SimpleDate.validDate?(3, 31, 2000))
  end  
  
  def test_validDate_T8() 
    assert(SimpleDate.validDate?(4, 30, 2000))
  end
  
  def test_validDate_T9() 
    assert(SimpleDate.validDate?(5, 31, 2010))
  end
  
  def test_validDate_T10() 
    assert(SimpleDate.validDate?(6, 30, 2006))
  end  
  
  def test_validDate_T11() 
    assert(SimpleDate.validDate?(7, 31, 2007))
  end
  
  def test_validDate_T12() 
    assert(SimpleDate.validDate?(8, 31, 2008))
  end  
        
  def test_validDate_T13() 
    assert(SimpleDate.validDate?(9, 30, 2002))
  end  

  def test_validDate_T14() 
    assert(SimpleDate.validDate?(10, 31, 1997))
  end  
  
  def test_validDate_T15() 
    assert(SimpleDate.validDate?(11, 30, 1992))
  end
  
  def test_validDate_T16() 
    assert(SimpleDate.validDate?(12, 31, 2034))
  end    
  
  def test_validDate_T17() 
    assert(!SimpleDate.validDate?(-1, 1, 2010))
  end 
  
  def test_validDate_T18() 
    assert(!SimpleDate.validDate?(0, 1, 2010))
  end
  
  def test_validDate_T19() 
    assert(!SimpleDate.validDate?(1, 0, 2010))
  end
  
  def test_validDate_T20()
    assert(!SimpleDate.validDate?(1, -3, 2010))
  end
  
  def test_validDate_T21()
    assert(!SimpleDate.validDate?(1, 1, 1751))
  end
  
  def test_validDate_T22()
    assert(!SimpleDate.validDate?(1, 7, -2023))
  end
  
  def test_validDate_T23()
    assert(!SimpleDate.validDate?(1, 32, 2010))
  end
  
  def test_validDate_T24()
    assert(!SimpleDate.validDate?(2, 29, 2009))
  end
  
  def test_validDate_T25()
    assert(!SimpleDate.validDate?(2, 29, 1900))
  end
  
  def test_validDate_T26()
    assert(!SimpleDate.validDate?(2, 30, 2009))
  end
  
  def test_validDate_T27()
    assert(!SimpleDate.validDate?(2, 30, 2012))
  end
  
  def test_validDate_T28()
    assert(!SimpleDate.validDate?(3, 32, 2000))
  end
  
  def test_validDate_T29()
    assert(!SimpleDate.validDate?(4, 31, 2000))
  end
  
  def test_validDate_T30()
    assert(!SimpleDate.validDate?(5, 32, 2000))
  end
  
  def test_validDate_T31()
    assert(!SimpleDate.validDate?(6, 31, 2000))
  end
  
  def test_validDate_T32()
    assert(!SimpleDate.validDate?(7, 32, 2000))
  end
  
  def test_validDate_T33()
    assert(!SimpleDate.validDate?(8, 32, 2000))
  end
  
  def test_validDate_T34()
    assert(!SimpleDate.validDate?(9, 31, 2000))
  end
  
  def test_validDate_T35()
    assert(!SimpleDate.validDate?(10, 32, 2000))
  end
  
  def test_validDate_T36()
    assert(!SimpleDate.validDate?(11, 31, 2000))
  end
  
  def test_validDate_T37()
    assert(!SimpleDate.validDate?(12, 32, 2000))
  end
    
  #
  # Unit tests for each() method
  #
  def test_each_T1()
    d = SimpleDate.new(10,15,2010)
    i = 1
    d.each() do |x|
      if i == 1
        assert_equal(10, x)
      elsif i == 2
        assert_equal(15, x)
      else
        assert_equal(2010, x)
      end
      i += 1
    end
  end
  
  def test_each_T2()
    d = SimpleDate.new(1,1,2012)
    i = 1
    d.each() do |x|
      if i == 1
        assert_equal(1, x)
      elsif i == 2
        assert_equal(1, x)
      else
        assert_equal(2012, x)
      end
      i += 1
    end
  end 
  
  def test_each_T3()
    d = SimpleDate.new(12,31,2012)
    i = 1
    d.each() do |x|
      if i == 1
        assert_equal(12, x)
      elsif i == 2
        assert_equal(31, x)
      else
        assert_equal(2012, x)
      end
      i += 1
    end
  end
     
end