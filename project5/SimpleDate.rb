#
# SimpleDate class represents a calendar date and contains methods to analyze dates.
#
# Author: Lawrence O'Boyle && Megan Maher
#
class SimpleDate
  
  include Comparable, Enumerable
  
  # define some useful constants
  MIN_YEAR = 1753   # minimum allowable year in a date
  NUM_MONTHS = 12
  DAYS_YEAR = 365
  DAYS_LEAP_YEAR = 366
  DAYS_IN_MONTH = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
  DAYS_THUS_FAR = [0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334]
  
  # define few more useful constants representing day of week
  SUNDAY = 0
  MONDAY = 1
  TUESDAY = 2
  WEDNESDAY = 3
  THURSDAY = 4
  FRIDAY = 5
  SATURDAY = 6
  
  # generates getter methods for @month, @day, and @year instance variables
  attr_reader :month, :day, :year
  
  #
  # Initializes the instance variables @month, @day, and @year using the values
  # of parameters.
  #
  # Raises ArgumentError exception when the following is true:
  #     month, day, and year are not Fixnum type, or
  #     values of month, day, and year do not represent a valid date
  #
  def initialize(month, day, year)
		if !(month.is_a?(Fixnum) && day.is_a?(Fixnum) && year.is_a?(Fixnum))
			raise ArgumentError.new("Invalid Type")
		end
		if !self.class.validDate?(month, day, year)
			raise ArgumentError.new("Invalid Date")
		end
		@month = month
		@day = day
		@year = year
		@data = [@month, @day, @year]
  end
  
  #
  # yields @month, @day, and @year values one at a time in that order
  #
  def each
		if block_given?
      @data.each{|x| yield(x)} 
    else    
      return @data.each
    end	
  end
  
  # 
  # Defines an ordering between this SimpleDate object and other SimpleDate object.
  # Returns a negative, zero, or postive number depending on whether the receiver
  # is less than, equal to, or greater than the other object. If the other
  # object is not comparable then the <=> operator should return nil.
  #
  def <=>(other)
		if !other.is_a?(SimpleDate) then return  nil end
		if self.year > other.year then return 1 end
		if self.year < other.year then return -1 end
		if self.month > other.month then return 1 end
		if self.month < other.month then return -1 end
		if self.day > other.day then return 1 end
		if self.day < other.day then return -1 end
		return 0
  end
  
  #
  # Returns an integer (must be same as constants SUNDAY, MONDAY, etc.) representing
  # the day of the week for this date.
  #
  def dayOfWeek
		t = [0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4]
		y = @year
		if @month < 3
			z = 1
		else
			z = 0
		end
		y = y - z
		return (y + (y / 4) - (y / 100) + (y / 400) + t[@month - 1] + @day) % 7
  end
  
  #
  # Returns the number of days in the year of this date
  #
  def daysInYear
		return self.class.daysInYear(@year)
	end
  
  #
  # Returns true if this date is in a leap year, false otherwise
  #
  def leapYear?
		return self.class.leapYear?(@year)
  end
  
  #
  # Returns the number of days that have elapsed (including this day) since 1 January.
  #
  def ordinalDate
		count = 1
		elapsed = 0
		while count < @month
			elapsed += self.class.daysInMonth(count, @year)
			count += 1
		end
		elapsed += @day
		return elapsed
  end
  
  #
  # Returns a SimpleDate object representing the next date of this date.
  #
  def nextDate    
		if (@day + 1) > self.class.daysInMonth(@month, @year)
			if (@month + 1) > 12
				return SimpleDate.new(1, 1, @year + 1)
			end
			return SimpleDate.new(@month + 1, 1, @year)
		end
		return SimpleDate.new(@month, @day + 1, @year)
  end
  
  #
  # Returns a SimpleDate object representing the previous date of this date or nil if
  # previous date is before the MIN_YEAR (i.e.. 1753).
  #
  def prevDate
		if (@day - 1) < 1
			if (@month - 1) < 1
				if (@year - 1) < MIN_YEAR
					return nil
				end
				return SimpleDate.new(12, 31, @year -1)
			end
			return SimpleDate.new(@month - 1, self.class.daysInMonth(@month - 1, @year), @year)
		end
		return SimpleDate.new(@month, @day - 1, @year)
  end
  
  #
  # Returns a new SimpleDate object representing the date n days ago.
  # If n is negative, then return the date n days in the future.
  # Raise ArgumentError if the new date is before the minimum allowable date (1/1/1753).
  #
  def daysAgo(n)
		if n == 0
			return SimpleDate.new(@month, @day, @year)
		end
		if n < 0
			return daysFromNow(n * -1)
		else
			new = prevDate()
			for z in 1...n do
				if new == nil
					raise ArgumentError.new("Too early")
				end
					new = new.prevDate()
			end
			return new
		end
  end
  
  #
  # Returns a new SimpleDate object representing the date n days from now.
  # If n is negative, then return the date n days in the past.
  # Raise ArgumentError if the new date is before the minimum allowable date (1/1/1753).
  #
  def daysFromNow(n)
		if n == 0
			return SimpleDate.new(@month, @day, @year)
		end 
		if n < 0
			return daysAgo(n * -1)
		else
			new = nextDate()
			for z in 1...n do
				new = new.nextDate()
			end
			return new
		end
  end
  
  #
  # Returns a string representation of this SimpleDate object.
  #
  def to_s
    return "#{@month}/#{@day}/#{@year}"
  end
  
  #
  # Class method that returns true if the given year is a leap year, false otherwise. 
  #
  def self.leapYear?(year)
		if year % 400 == 0
			return true
		elsif year % 100 == 0
			return false
    else
      return year % 4 == 0
		end
  end

  #
  # Class method that returns the number of days in the given year.
  #
  def self.daysInYear(year)
		if leapYear?(year)
			return DAYS_LEAP_YEAR
		end
		return DAYS_YEAR
  end
 
  #
  # Class method that returns the number of days in a month for a given year.
  #
  def self.daysInMonth(month, year)
    if leapYear?(year) && month == 2
			return 29
		else
			return DAYS_IN_MONTH[month]
		end
  end
  
  #
  # Class method that determines if values for month, day, and year represent a valid date.
  #
  def self.validDate?(month, day, year)
    if month <= 0 || month > 12
			return false
		end
		if year < MIN_YEAR
			return false
		end
		if day < 1 || day > daysInMonth(month, year)
			return false
		end
		return true
  end
  
end   # end of SimpleDate class
